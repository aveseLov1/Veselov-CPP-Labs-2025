#include "matrix_operations.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

// Реализация функций из пространства имен MatrixModule

namespace MatrixModule {

double** allocateMatrix(int numberOfRows, int numberOfColumns) {
    double** matrix = new double*[numberOfRows];
    for (int i = 0; i < numberOfRows; ++i) {
        matrix[i] = new double[numberOfColumns];
    }
    return matrix;
}

void freeMatrix(double** matrix, int numberOfRows) {
    for (int i = 0; i < numberOfRows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

void fillMatrix(double** matrix, int numberOfRows, int numberOfColumns, double x) {
    for (int i = 0; i < numberOfRows; ++i) {
        for (int j = 0; j < numberOfColumns; ++j) {
            if (i == j) {
                // Главная диагональ
                matrix[i][j] = 1.0;
            } else if (j > i) {
                // Выше главной диагонали: ai,j = x^i / (j!)^i
                double power = std::pow(x, i + 1);
                long long fact = factorial(j + 1);
                double denominator = std::pow(static_cast<double>(fact), i + 1);
                matrix[i][j] = power / denominator;
            } else {
                // Ниже главной диагонали: ai,j = (-x)^i / (j!)^i
                double power = std::pow(-x, i + 1);
                long long fact = factorial(j + 1);
                double denominator = std::pow(static_cast<double>(fact), i + 1);
                matrix[i][j] = power / denominator;
            }
        }
    }
}

// Функция вывода динамической матрицы
void printDynamicMatrix(double** matrix, int numberOfRows, int numberOfColumns, int precision, bool scientificFormat, int maxLineLength) {
    // Сохраняем оригинальные флаги формата
    std::ios_base::fmtflags originalFlags = std::cout.flags();
    if (scientificFormat) {
        std::cout << std::scientific;
    } else {
        std::cout << std::fixed;
    }

    std::cout << std::setprecision(precision);

    // Рассчитываем максимальное количество столбцов в одной строке вывода
    int elementWidth;
    if (scientificFormat) {
        // Формат: ±X.XXXXXXe±XX (примерно 13 символов)
        elementWidth = precision + 9;
    } else {
        // Формат: ±X.XXXXXX (примерно precision + 3 символов)
        elementWidth = precision + 5;
    }

    // Добавляем пробелы между элементами
    elementWidth += 3;

    // Вычисляем, сколько столбцов поместится в строку
    int colsPerLine = maxLineLength / elementWidth;
    if (colsPerLine < 1)
        colsPerLine = 1;

    // Вывод матрицы с возможным переносом
    for (int startCol = 0; startCol < numberOfColumns; startCol += colsPerLine) {
        int endCol = std::min(startCol + colsPerLine, numberOfColumns);

        std::cout << "\nСтолбцы " << startCol + 1 << "-" << endCol << ":\n";

        for (int i = 0; i < numberOfRows; ++i) {
            for (int j = startCol; j < endCol; ++j) {
                std::cout << std::setw(elementWidth - 3) << matrix[i][j] << "   ";
            }
            std::cout << "\n";
        }
    }

    // Восстанавливаем оригинальные флаги
    std::cout.flags(originalFlags);
    std::cout << std::defaultfloat;
}

// Функция вывода статической матрицы через вспомогательный массив указателей
void printStaticMatrix(double* rows[], int numberOfRows, int numberOfColumns, int precision, bool scientificFormat, int maxLineLength) {
    // Сохраняем оригинальные флаги формата
    std::ios_base::fmtflags originalFlags = std::cout.flags();
    if (scientificFormat) {
        std::cout << std::scientific;
    } else {
        std::cout << std::fixed;
    }

    std::cout << std::setprecision(precision);

    // Рассчитываем максимальное количество столбцов в одной строке вывода
    int elementWidth;
    if (scientificFormat) {
        elementWidth = precision + 9;
    } else {
        elementWidth = precision + 5;
    }
    elementWidth += 3;

    int colsPerLine = maxLineLength / elementWidth;
    if (colsPerLine < 1)
        colsPerLine = 1;

    // Вывод матрицы с возможным переносом
    for (int startCol = 0; startCol < numberOfColumns; startCol += colsPerLine) {
        int endCol = std::min(startCol + colsPerLine, numberOfColumns);

        std::cout << "\nСтолбцы " << startCol + 1 << "-" << endCol << ":\n";

        for (int i = 0; i < numberOfRows; ++i) {
            for (int j = startCol; j < endCol; ++j) {
                std::cout << std::setw(elementWidth - 3) << rows[i][j] << "   ";
            }
            std::cout << "\n";
        }
    }

    // Восстанавливаем оригинальные флаги
    std::cout.flags(originalFlags);
    std::cout << std::defaultfloat;
}
}  // namespace MatrixModule
