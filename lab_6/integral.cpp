#include "integral.h"

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

        // Проверка точности по правилу Рунге
        // Для метода прямоугольников погрешность ~ I_curr - I_prev
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

        // Для метода трапеций погрешность ~ (I_curr - I_prev)/3
    } while (fabs(I_curr - I_prev) > 3*eps);

    return I_curr;
}

// Функция для печати таблицы
namespace {
    const int numberOfTableColumns = 4;
    const int maxWidthOfTableColumns = 18;

    const int firstColumnWidth = 12;
    const int secondColumnWidth = 18;
    const int thirdColumnWidth = 18;
    const int fourthColumnWidth = 10;

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

void printTabl(resultToPrint* i_prn, int countRowOfTable) {
    int widthOfTableColumns[numberOfTableColumns] = {
        firstColumnWidth, secondColumnWidth,
        thirdColumnWidth, fourthColumnWidth
    };

    char* title[numberOfTableColumns];
    title[0] = new char[std::strlen("  Function  ") + 1];
    std::strcpy(title[0], "  Function  ");
    title[1] = new char[std::strlen("     Integral     ") + 1];
    std::strcpy(title[1], "     Integral     ");
    title[2] = new char[std::strlen("      IntSum      ") + 1];
    std::strcpy(title[2], "      IntSum      ");
    title[3] = new char[std::strlen("    N     ") + 1];
    std::strcpy(title[3], "    N     ");

    int size[numberOfTableColumns];
    for (int i = 0; i < numberOfTableColumns; ++i)
        size[i] = std::strlen(title[i]);

    // Верхняя рамка
    std::cout << ul << std::setfill('-');
    for (int j = 0; j < numberOfTableColumns - 1; ++j)
        std::cout << std::setw(widthOfTableColumns[j] + 3) << Td;
    std::cout << std::setw(widthOfTableColumns[numberOfTableColumns - 1] + 3) << ur << std::endl;

    // Заголовки
    std::cout << vt;
    for (int j = 0; j < numberOfTableColumns; ++j)
        std::cout << title[j] << vt;
    std::cout << std::endl;

    // Тело таблицы
    for (int i = 0; i < countRowOfTable; ++i) {
        std::cout << Tr << std::setfill('-');
        for (int j = 0; j < numberOfTableColumns - 1; ++j)
            std::cout << std::setw(widthOfTableColumns[j] + 3) << cr;
        std::cout << std::setw(widthOfTableColumns[numberOfTableColumns - 1] + 3)
            << Tl << std::setfill(' ') << std::endl;

        std::cout << vt
            << std::setw((widthOfTableColumns[0] - std::strlen(i_prn[i].name)) / 2) << ' '
            << i_prn[i].name
            << std::setw((widthOfTableColumns[0] - std::strlen(i_prn[i].name)) / 2) << vt;

        std::cout << std::setw(widthOfTableColumns[1]) << std::setprecision(6)
            << i_prn[i].i_toch << vt
            << std::setw(widthOfTableColumns[2])
            << i_prn[i].i_sum << vt
            << std::setw(widthOfTableColumns[3])
            << i_prn[i].n << vt << std::endl;
    }

    // Нижняя рамка
    std::cout << dl << std::setfill('-');
    for (int j = 0; j < numberOfTableColumns - 1; ++j)
        std::cout << std::setw(widthOfTableColumns[j] + 3) << Tu;
    std::cout << std::setw(widthOfTableColumns[numberOfTableColumns - 1] + 3)
        << dr << std::setfill(' ') << std::endl;

    // Освобождение памяти
    for (int i = 0; i < numberOfTableColumns; ++i)
        delete[] title[i];
}
