#ifndef INTEGRAL_H
#define INTEGRAL_H

#include <cmath>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>

// Тип указателя на функцию
typedef double (*TPF)(double);

// Структура для хранения результатов
struct resultToPrint {
    char* name;     // название функции
    double i_sum;   // численно рассчитанный интеграл
    double i_toch;  // точное значение интеграла
    int n;          // число разбиений при достижении точности
};

// Подынтегральные функции
double f1(double x);
double f2(double x);
double f3(double x);
double f4(double x);

// Точные значения интегралов
double exact_integral_f1(double a, double b);
double exact_integral_f2(double a, double b);
double exact_integral_f3(double a, double b);
double exact_integral_f4(double a, double b);

// Метод прямоугольников
double integrationByRectangle(TPF f, double a, double b, double eps, int& n);

// Метод трапеций
double integrationByTrapezoidal(TPF f, double a, double b, double eps, int& n);

// Функция печати таблицы
void printTabl(resultToPrint* i_prn, int countRowOfTable, double eps);

#endif  // INTEGRAL_H
