#include "integral.hpp"

using namespace std;

int main() {
    // Пределы интегрирования
    double a = 0.0;
    double b = 1.0;

    // Массив значений точности
    double epsilons[] = { 0.01, 0.001, 0.0001, 0.00001, 0.000001 };
    int num_eps = 5;

    // Массив указателей на функции
    TPF functions[] = { f1, f2, f3, f4 };
    char* func_names[] = {
        new char[3]{'x', '\0'},
        new char[10]{'s','i','n','(','2','2','x',')','\0'},
        new char[6]{'x','^','4','\0'},
        new char[10]{'a','r','c','t','g','(','x',')','\0'}
    };
    int num_funcs = 4;

    // Массив указателей на функции точных значений
    double (*exact_funcs[])(double, double) = {
        exact_integral_f1, exact_integral_f2, exact_integral_f3, exact_integral_f4
    };

    // Метод прямоугольников
    cout << "==================================================" << endl;
    cout << "          МЕТОД ПРЯМОУГОЛЬНИКОВ" << endl;
    cout << "==================================================" << endl;

    for (int e = 0; e < num_eps; e++) {
        double eps = epsilons[e];
        cout << "\nТочность: " << eps << endl;

        resultToPrint results[4];
        for (int i = 0; i < num_funcs; i++) {
            int n = 0;
            double integral = integrationByRectangle(functions[i], a, b, eps, n);
            double exact = exact_funcs[i](a, b);

            results[i].name = func_names[i];
            results[i].i_sum = integral;
            results[i].i_toch = exact;
            results[i].n = n;
        }

        printTabl(results, 4);
    }

    // Метод трапеций
    cout << "\n\n==================================================" << endl;
    cout << "             МЕТОД ТРАПЕЦИЙ" << endl;
    cout << "==================================================" << endl;

    for (int e = 0; e < num_eps; e++) {
        double eps = epsilons[e];
        cout << "\nТочность: " << eps << endl;

        resultToPrint results[4];
        for (int i = 0; i < num_funcs; i++) {
            int n = 0;
            double integral = integrationByTrapezoidal(functions[i], a, b, eps, n);
            double exact = exact_funcs[i](a, b);

            results[i].name = func_names[i];
            results[i].i_sum = integral;
            results[i].i_toch = exact;
            results[i].n = n;
        }

        printTabl(results, 4);
    }

    // Освобождение памяти
    for (int i = 0; i < num_funcs; i++) {
        delete[] func_names[i];
    }

    return 0;
}
