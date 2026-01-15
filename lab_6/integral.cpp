#include "integral.hpp"
#include <sstream>
#include <cmath>

// Подынтегральные функции
double f1(double x) { return x; }
double f2(double x) { return sin(22 * x); }
double f3(double x) { return x * x * x * x; }
double f4(double x) { return atan(x); }

// Точные значения интегралов
double exact_integral_f1(double a, double b) {
    return (b*b - a*a)/2.0;
}

double exact_integral_f2(double a, double b) {
    return (cos(22*a) - cos(22*b)) / 22.0;
}

double exact_integral_f3(double a, double b) {
    return (pow(b,5) - pow(a,5)) / 5.0;
}

double exact_integral_f4(double a, double b) {
    return b*atan(b) - a*atan(a) - (log(b*b+1) - log(a*a+1)) / 2.0;
}

// Метод прямоугольников
double integrationByRectangle(TPF f, double a, double b, double eps, int& n) {
    n = 2;
    double I_prev, I_curr;
    double dx;

    // Первое вычисление с n разбиениями
    dx = (b - a) / n;
    I_curr = 0;
    for (int i = 0; i < n; i++) {
        double x_mid = a + (i + 0.5) * dx; // середина прямоугольника
        I_curr += f(x_mid);
    }
    I_curr *= dx;

    // Уточнение по правилу Рунге
    do {
        I_prev = I_curr;
        n *= 2;
        dx = (b - a) / n;
        I_curr = 0;

        for (int i = 0; i < n; i++) {
            double x_mid = a + (i + 0.5) * dx;
            I_curr += f(x_mid);
        }
        I_curr *= dx;

    } while (fabs(I_curr - I_prev) > eps);

    return I_curr;
}

// Метод трапеций
double integrationByTrapezoidal(TPF f, double a, double b, double eps, int& n) {
    n = 2;
    double I_prev, I_curr;
    double dx;

    // Первое вычисление с n разбиениями
    dx = (b - a) / n;
    I_curr = (f(a) + f(b)) / 2.0;
    for (int i = 1; i < n; i++) {
        double x = a + i * dx;
        I_curr += f(x);
    }
    I_curr *= dx;

    // Уточнение по правилу Рунге
    do {
        I_prev = I_curr;
        n *= 2;
        dx = (b - a) / n;

        I_curr = (f(a) + f(b)) / 2.0;
        for (int i = 1; i < n; i++) {
            double x = a + i * dx;
            I_curr += f(x);
        }
        I_curr *= dx;

    } while (fabs(I_curr - I_prev) > 3*eps);

    return I_curr;
}

// Функция для печати таблицы (ИСПРАВЛЕНА - точность вывода = точности вычислений)
namespace {
    const int numberOfTableColumns = 4;

    const int firstColumnWidth = 10;
    const int secondColumnWidth = 16;
    const int thirdColumnWidth = 16;
    const int fourthColumnWidth = 8;

    const char* ul = "┌";
    const char* ur = "┐";
    const char* dl = "└";
    const char* dr = "┘";
    const char* vt = "│";
    const char* cr = "┼";
    const char* Td = "┬";
    const char* Tu = "┴";
    const char* Tr = "├";
    const char* Tl = "┤";
}

void printTabl(resultToPrint* i_prn, int countRowOfTable, double eps) {
    int widthOfTableColumns[numberOfTableColumns] = {
        firstColumnWidth, secondColumnWidth,
        thirdColumnWidth, fourthColumnWidth
    };

    char* title[numberOfTableColumns];
    title[0] = new char[std::strlen(" Function ") + 1];
    std::strcpy(title[0], " Function ");
    title[1] = new char[std::strlen(" Integral ") + 1];
    std::strcpy(title[1], " Integral ");
    title[2] = new char[std::strlen(" IntSum ") + 1];
    std::strcpy(title[2], " IntSum ");
    title[3] = new char[std::strlen(" N ") + 1];
    std::strcpy(title[3], " N ");

    // Верхняя рамка
    std::cout << ul;
    for (int j = 0; j < numberOfTableColumns; ++j) {
        for (int k = 0; k < widthOfTableColumns[j] + 2; ++k) std::cout << "─";
        if (j < numberOfTableColumns - 1) std::cout << Td;
    }
    std::cout << ur << std::endl;

    // Заголовки
    std::cout << vt;
    for (int j = 0; j < numberOfTableColumns; ++j) {
        int totalSpaces = widthOfTableColumns[j] + 2 - std::strlen(title[j]);
        int leftSpaces = totalSpaces / 2;
        int rightSpaces = totalSpaces - leftSpaces;

        std::cout << std::string(leftSpaces, ' ')
                  << title[j]
                  << std::string(rightSpaces - 1, ' ');
    }
    std::cout << vt << std::endl;

    // Разделитель заголовка и данных
    std::cout << Tr;
    for (int j = 0; j < numberOfTableColumns; ++j) {
        for (int k = 0; k < widthOfTableColumns[j] + 2; ++k) std::cout << "─";
        if (j < numberOfTableColumns - 1) std::cout << cr;
    }
    std::cout << Tl << std::endl;

    // Тело таблицы
    for (int i = 0; i < countRowOfTable; ++i) {
        std::cout << vt;

        // Столбец 1: Название функции (центрируем)
        int func_len = std::strlen(i_prn[i].name);
        int func_spaces = widthOfTableColumns[0] + 2 - func_len;
        int func_left = func_spaces / 2;
        int func_right = func_spaces - func_left;
        std::cout << std::string(func_left, ' ')
                  << i_prn[i].name
                  << std::string(func_right - 1, ' ')
                  << vt;

        // Определяем количество знаков после запятой на основе epsilon
        // eps = 0.01 -> 2 знака, eps = 0.001 -> 3 знака, и т.д.
        int precision;
        if (eps >= 0.1) {
            precision = 1;        // eps = 0.1 -> 1 знак
        } else if (eps >= 0.01) {
            precision = 2;        // eps = 0.01 -> 2 знака
        } else if (eps >= 0.001) {
            precision = 3;        // eps = 0.001 -> 3 знака
        } else if (eps >= 0.0001) {
            precision = 4;        // eps = 0.0001 -> 4 знака
        } else if (eps >= 0.00001) {
            precision = 5;        // eps = 0.00001 -> 5 знаков
        } else {
            precision = 6;        // eps = 0.000001 -> 6 знаков
        }

        // Столбец 2: Точное значение интеграла
        double exact_val = i_prn[i].i_toch;
        std::ostringstream oss1;

        // Используем нужную точность
        oss1 << std::fixed << std::setprecision(precision) << exact_val;

        std::string exact_str = oss1.str();
        std::cout << " " << std::right << std::setw(widthOfTableColumns[1] + 1)
                  << exact_str << vt;

        // Столбец 3: Численное значение интеграла
        double sum_val = i_prn[i].i_sum;
        std::ostringstream oss2;

        oss2 << std::fixed << std::setprecision(precision) << sum_val;

        std::string sum_str = oss2.str();
        std::cout << " " << std::right << std::setw(widthOfTableColumns[2] + 1)
                  << sum_str << vt;

        // Столбец 4: Количество разбиений
        std::cout << " " << std::right << std::setw(widthOfTableColumns[3] + 1)
                  << i_prn[i].n << vt;

        std::cout << std::endl;
    }

    // Нижняя рамка
    std::cout << dl;
    for (int j = 0; j < numberOfTableColumns; ++j) {
        for (int k = 0; k < widthOfTableColumns[j] + 2; ++k) std::cout << "─";
        if (j < numberOfTableColumns - 1) std::cout << Tu;
    }
    std::cout << dr << std::endl;

    // Освобождение памяти
    for (int i = 0; i < numberOfTableColumns; ++i)
        delete[] title[i];
}
