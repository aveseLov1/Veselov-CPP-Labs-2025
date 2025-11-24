#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

using namespace std;

// Задача 1: Сумма чисел, делящихся на 5 и не делящихся на m
void task1() {
    cout << "=== ЗАДАЧА 1 ===" << endl;
    int n, m;

    cout << "Введите n: ";
    cin >> n;
    cout << "Введите m (m < n): ";
    cin >> m;

    if (m >= n) {
        cout << "Ошибка: m должно быть меньше n!" << endl;
        return;
    }

    int sum = 0;
    for (int i = 5; i <= n; i = i + 5) {
        if (i % m != 0) {
            sum += i;
        }
    }
    cout << "Сумма чисел: " << sum << endl;
}

// Задача 2: Вычисление произведения в зависимости от a
void task2() {
    double a = 0.;
    cout << "Введите число a:" << '\n';
    cin >> a;

    double sum = 1.;
    if (a >= 0) {
        const int kMinI = 2;
        const int kMaxI = 8;
        const int kStepI = 2;
        int i = kMinI;
        while (i <= kMaxI) {
            sum *= i * i;
            i += kStepI;
        }
        sum -= a;
    } else {
        const int kMinI = 3;
        const int kMaxI = 9;
        const int kStepI = 3;
        const int kTwo = 2;
        int i = kMinI;
        while (i <= kMaxI) {
            sum *= (i - kTwo);
            i += kStepI;
        }
    }
    cout << "Ответ на задачу: " << fixed << sum << '\n';
}

// Задача 3: Сумма ряда Тейлора для e^(2x) с использованием рекуррентного соотношения
void task3() {
    cout << "=== ЗАДАЧА 3 ===" << endl;
    const double epsilon = 1e-6;
    const double h = 0.2;

    cout << fixed << setprecision(6);
    cout << setw(10) << "x" << setw(15) << "Y(x)" << setw(15) << "S(x)" << setw(8) << "N" << endl;
    cout << string(48, '-') << endl;

    for (double x = 0; x <= 1.01; x += h) {
        double Y = exp(2 * x);  // Точное значение функции
        double S = 1.0;         // Сумма ряда (первый член = 1)
        double term = 1.0;      // Текущий член ряда
        int N = 0;              // Номер последнего слагаемого

        // Вычисление суммы ряда с использованием рекуррентного соотношения
        for (int n = 1; n <= 10000000000; n++) {
            // Рекуррентное соотношение: term_n = term_{n-1} * (2x / n)
            term *= (2 * x) / n;
            S += term;

            if (fabs(term) < epsilon) {
                N = n;
                break;
            }

            // Защита от бесконечного цикла
            if (n == 1000) {
                N = 1000;
                break;
            }
        }

        cout << setw(10) << x << setw(15) << Y << setw(15) << S << setw(8) << N << endl;
    }
}

// Задача 4: Вычисление знакопеременного ряда с промежуточными суммами
void task4() {
    cout << "=== ЗАДАЧА 4 ===" << endl;
    int n;
    double x;

    cout << "Введите n: ";
    cin >> n;
    cout << "Введите x: ";
    cin >> x;

    double y = 1.0;             // Первый член ряда (n=0)
    double current_term = 1.0;  // Текущий член ряда
    double numerator = 1.0;     // Текущий числитель
    double denominator = 1.0;   // Текущий знаменатель

    cout << fixed << setprecision(6);
    cout << "Промежуточные суммы:" << endl;

    for (int i = 1; i <= n; i++) {
        // Рекуррентное обновление числителя и знаменателя
        numerator *= (2 * i + 1);  // 3*5*7*...*(2n+1)
        denominator *= (2 * i);    // 2*4*6*...*(2n)

        // Вычисление текущего члена ряда с использованием предыдущего
        // term_i = term_{i-1} * (-1) * x * (2i+1) / (2i)
        current_term *= (-1) * x * (2 * i + 1) / (2 * i);
        y += current_term;

        // Вывод промежуточных сумм
        if (i == 2) {
            cout << "Сумма при 3 слагаемых: " << y << endl;
            // } else if (i == 4) {
            //     cout << "Сумма при 5 слагаемых: " << y << endl;
        } else if (i == 9) {
            cout << "Сумма при 10 слагаемых: " << y << endl;
        }
    }

    cout << "Итоговая сумма при " << n << " слагаемых: " << y << endl;
}

// Функция для очистки буфера ввода
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Главное меню
void showMenu() {
    cout << "\n=== ГЛАВНОЕ МЕНЮ ===" << endl;
    cout << "1. Задача 1 - Сумма чисел, делящихся на 5 и не делящихся на m" << endl;
    cout << "2. Задача 2 - Вычисление произведения" << endl;
    cout << "3. Задача 3 - Ряд Тейлора для e^(2x)" << endl;
    cout << "4. Задача 4 - Знакопеременный ряд" << endl;
    cout << "0. Выход" << endl;
    cout << "Выберите задание: ";
}

int main() {
    // Установка русской локализации для вывода
    setlocale(LC_ALL, "Russian");

    int choice;
    char continueChoice;

    do {
        showMenu();
        cin >> choice;
        clearInputBuffer();

        switch (choice) {
            case 1:
                task1();
                break;
            case 2:
                task2();
                break;
            case 3:
                task3();
                break;
            case 4:
                task4();
                break;
            case 0:
                cout << "Выход из программы." << endl;
                return 0;
            default:
                cout << "Неверный выбор! Попробуйте снова." << endl;
                break;
        }

        cout << "\nПродолжить работу? (y/n): ";
        cin >> continueChoice;
        clearInputBuffer();

    } while (continueChoice == 'y' || continueChoice == 'Y');

    cout << "Программа завершена." << endl;
    return 0;
}

// #include <iostream>
// #include <iomanip>
// #include <cmath>

// using namespace std;

// // Задача 1: Сумма чисел, делящихся на 5 и не делящихся на m
// void task1() {
//     cout << "=== ЗАДАЧА 1 ===" << endl;
//     int n, m;
//     cout << "Введите n: ";
//     cin >> n;
//     cout << "Введите m (m < n): ";
//     cin >> m;

//     if (m >= n) {
//         cout << "Ошибка: m должно быть меньше n!" << endl;
//         return;
//     }

//     int sum = 0;
//     cout << "Числа, удовлетворяющие условию: ";
//     for (int i = 1; i <= n; i++) {
//         if (i % 5 == 0 && i % m != 0) {
//             cout << i << " ";
//             sum += i;
//         }
//     }
//     cout << endl << "Сумма: " << sum << endl << endl;
// }

// // Задача 2: Вычисление S в зависимости от a
// void task2() {
//     double a;
//     cout << "Введите a: ";
//     cin >> a;

//     double S;
//     if (a >= 0) {
//         S = 1.0;
//         for (int i = 2; i <= 8; i++) {
//             S *= i * i;
//         }
//         S -= a;
//     } else {
//         S = 1.0;
//         for (int i = 3; i <= 9; i += 3) {
//             S *= (i - 2);
//         }
//     }

//     cout << "S = " << S << endl;
// }

// long long factorial(int n) {
//     long long result = 1;
//     for (int i = 2; i <= n; i++) {
//         result *= i;
//     }
//     return result;
// }

// // Задача 3: Ряд Тейлора для e^(2x)
// void task3() {
//     cout << "=== ЗАДАЧА 3 ===" << endl;
//     cout << "x\tY(x)\t\tS(x)\t\tN" << endl;
//     cout << fixed << setprecision(6);

//     for (double x = 0; x <= 1.01; x += 0.2) {
//         double Y = exp(2 * x);  // Точное значение функции
//         double S = 1.0;         // Сумма ряда (начинается с 1)
//         double term = 1.0;      // Текущий член ряда
//         int n = 0;              // Номер текущего члена
//         int lastN = 0;          // Номер последнего слагаемого

//         // Вычисляем сумму ряда с точностью 1e-6
//         while (fabs(term) > 1e-6 && n < 1000) {
//             n++;
//             // Вычисляем факториал в цикле без отдельной функции
//             long long fact = 1;
//             for (int i = 1; i <= n; i++) {
//                 fact *= i;
//             }
//             term = pow(2 * x, n) / fact;
//             S += term;
//             lastN = n;
//         }

//         cout << x << "\t" << Y << "\t" << S << "\t" << lastN << endl;
//     }
//     cout << endl;
// }

// // Задача 4: Вычисление y по заданной формуле
// void task4() {
//     cout << "=== ЗАДАЧА 4 ===" << endl;
//     int n;
//     double x;
//     cout << "Введите n: ";
//     cin >> n;
//     cout << "Введите x: ";
//     cin >> x;

//     double y = 1.0;  // Первый член ряда (при n=0)
//     double numerator = 1.0;    // Числитель текущего члена
//     double denominator = 1.0;  // Знаменатель текущего члена

//     cout << "Промежуточные суммы:" << endl;
//     cout << fixed << setprecision(6);

//     for (int i = 1; i <= n; i++) {
//         // Обновляем числитель: умножаем на следующее нечётное число
//         numerator *= (2 * i + 1);
//         // Обновляем знаменатель: умножаем на следующее чётное число
//         denominator *= (2 * i);
//         // Вычисляем текущий член ряда
//         double term = pow(-1, i) * (numerator / denominator) * pow(x, i);
//         y += term;

//         // Выводим промежуточные суммы для 3, 5 и 10 слагаемых
//         if (i == 2) {  // 3 слагаемых (индексы 0,1,2)
//             cout << "После 3 слагаемых: " << y << endl;
//         } else if (i == 4) {  // 5 слагаемых (индексы 0,1,2,3,4)
//             cout << "После 5 слагаемых: " << y << endl;
//         } else if (i == 9) {  // 10 слагаемых (индексы 0,...,9)
//             cout << "После 10 слагаемых: " << y << endl;
//         }
//     }

//     cout << "Финальный результат: " << y << endl << endl;
// }

// // Функция для отображения меню
// void showMenu() {
//     cout << "====================================" << endl;
//     cout << "           ВЫБОР ЗАДАЧИ" << endl;
//     cout << "====================================" << endl;
//     cout << "1. Сумма чисел, делящихся на 5 и не делящихся на m" << endl;
//     cout << "2. Вычисление S в зависимости от a" << endl;
//     cout << "3. Ряд Тейлора для e^(2x)" << endl;
//     cout << "4. Вычисление y по специальной формуле" << endl;
//     cout << "0. Выход" << endl;
//     cout << "====================================" << endl;
//     cout << "Выберите задачу (0-4): ";
// }

// int main() {
//     setlocale(LC_ALL, "Russian");

//     int choice;

//     do {
//         showMenu();
//         cin >> choice;

//         switch (choice) {
//             case 1:
//                 task1();
//                 break;
//             case 2:
//                 task2();
//                 break;
//             case 3:
//                 task3();
//                 break;
//             case 4:
//                 task4();
//                 break;
//             case 0:
//                 cout << "Выход из программы..." << endl;
//                 break;
//             default:
//                 cout << "Неверный выбор! Пожалуйста, выберите от 0 до 4." << endl << endl;
//                 break;
//         }

//         // Пауза перед очисткой экрана (для удобства)
//         if (choice != 0) {
//             cout << "Нажмите Enter для продолжения...";
//             cin.ignore();
//             cin.get();
//             system("clear"); // для Linux/Mac
//             // system("cls"); // для Windows
//         }

//     } while (choice != 0);

//     return 0;
// }
