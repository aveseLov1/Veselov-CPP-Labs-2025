#include "matrix_operations.hpp"
#include <climits>
#include <cmath>
#include <iomanip>
#include <iostream>

namespace MatrixModule {

double** allocateMatrix(std::size_t numberOfRows, std::size_t numberOfColumns) {
    double** matrix = new double*[numberOfRows];
    for (std::size_t i = 0; i < numberOfRows; ++i) {
        matrix[i] = new double[numberOfColumns]();  // Инициализация нулями
    }
    return matrix;
}

void freeMatrix(double** matrix, std::size_t numberOfRows) {
    if (matrix == nullptr) {
        return;
    }
    for (std::size_t i = 0; i < numberOfRows; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

long long factorial(int n) {
    if (n < 0) {
        return 0;
    }
    // Факториал 21! уже не помещается в long long
    if (n > 20) {
        return 0;  // Ошибка переполнения
    }

    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        // Проверка переполнения перед умножением
        if (result > LLONG_MAX / i) {
            return 0;  // Переполнение
        }
        result *= i;
    }
    return result;
}

void fillMatrix(double** matrix, std::size_t numberOfRows, std::size_t numberOfColumns, double x) {
    for (std::size_t i = 0; i < numberOfRows; ++i) {
        for (std::size_t j = 0; j < numberOfColumns; ++j) {
            if (i == j) {
                // Главная диагональ
                matrix[i][j] = 1.0;
            } else if (j > i) {
                // Выше главной диагонали: ai,j = x^i / (j!)^i
                double power = std::pow(x, static_cast<int>(i) + 1);
                if (std::isinf(power) || std::isnan(power)) {
                    matrix[i][j] = 0.0;
                    continue;
                }

                long long fact = factorial(static_cast<int>(j) + 1);
                if (fact == 0) {
                    matrix[i][j] = 0.0;
                    continue;
                }

                double denominator = std::pow(static_cast<double>(fact), static_cast<int>(i) + 1);
                if (denominator == 0.0 || std::isinf(denominator) || std::isnan(denominator)) {
                    matrix[i][j] = 0.0;
                } else {
                    matrix[i][j] = power / denominator;
                }
            } else {
                // Ниже главной диагонали: ai,j = (-x)^i / (j!)^i
                double power = std::pow(-x, static_cast<int>(i) + 1);
                if (std::isinf(power) || std::isnan(power)) {
                    matrix[i][j] = 0.0;
                    continue;
                }

                long long fact = factorial(static_cast<int>(j) + 1);
                if (fact == 0) {
                    matrix[i][j] = 0.0;
                    continue;
                }

                double denominator = std::pow(static_cast<double>(fact), static_cast<int>(i) + 1);
                if (denominator == 0.0 || std::isinf(denominator) || std::isnan(denominator)) {
                    matrix[i][j] = 0.0;
                } else {
                    matrix[i][j] = power / denominator;
                }
            }
        }
    }
}

void printDynamicMatrix(const double* const* matrix, std::size_t numberOfRows, std::size_t numberOfColumns, int precision, bool scientificFormat,
                        int maxLineLength) {
    if (matrix == nullptr || numberOfRows == 0 || numberOfColumns == 0) {
        std::cout << "Матрица пуста или не инициализирована.\n";
        return;
    }

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
    if (colsPerLine < 1) {
        colsPerLine = 1;
    }

    // Вывод матрицы с возможным переносом
    for (std::size_t startCol = 0; startCol < numberOfColumns; startCol += static_cast<std::size_t>(colsPerLine)) {
        std::size_t endCol = std::min(startCol + static_cast<std::size_t>(colsPerLine), numberOfColumns);

        std::cout << "\nСтолбцы " << startCol + 1 << "-" << endCol << ":\n";

        for (std::size_t i = 0; i < numberOfRows; ++i) {
            for (std::size_t j = startCol; j < endCol; ++j) {
                std::cout << std::setw(elementWidth - 3) << matrix[i][j] << "   ";
            }
            std::cout << "\n";
        }
    }

    // Восстанавливаем оригинальные флаги
    std::cout.flags(originalFlags);
    std::cout << std::defaultfloat;
}

void printStaticMatrix(const double* const rows[], std::size_t numberOfRows, std::size_t numberOfColumns, int precision, bool scientificFormat,
                       int maxLineLength) {
    if (rows == nullptr || numberOfRows == 0 || numberOfColumns == 0) {
        std::cout << "Матрица пуста или не инициализирована.\n";
        return;
    }

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
    if (colsPerLine < 1) {
        colsPerLine = 1;
    }

    // Вывод матрицы с возможным переносом
    for (std::size_t startCol = 0; startCol < numberOfColumns; startCol += static_cast<std::size_t>(colsPerLine)) {
        std::size_t endCol = std::min(startCol + static_cast<std::size_t>(colsPerLine), numberOfColumns);

        std::cout << "\nСтолбцы " << startCol + 1 << "-" << endCol << ":\n";

        for (std::size_t i = 0; i < numberOfRows; ++i) {
            for (std::size_t j = startCol; j < endCol; ++j) {
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
