#ifndef MATRIX_OPERATIONS_HPP
#define MATRIX_OPERATIONS_HPP

#include <cstddef>

namespace MatrixModule {

// Функция выделения памяти для двухмерного динамического массива
// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays, hicpp-avoid-c-arrays, modernize-avoid-c-arrays)
double** allocateMatrix(std::size_t numberOfRows, std::size_t numberOfColumns);

// Функция освобождения памяти
void freeMatrix(double** matrix, std::size_t numberOfRows);

// Функция вычисления факториала
long long factorial(int n);

// Функция заполнения матрицы по заданным формулам
void fillMatrix(double** matrix, std::size_t numberOfRows, std::size_t numberOfColumns, double x = 1.0);

// Функция вывода динамической матрицы
void printDynamicMatrix(const double* const* matrix, std::size_t numberOfRows, std::size_t numberOfColumns, int precision = 6,
                        bool scientificFormat = false, int maxLineLength = 80);

// Функция вывода статической матрицы через вспомогательный массив указателей
void printStaticMatrix(const double* const rows[], std::size_t numberOfRows, std::size_t numberOfColumns, int precision = 6,
                       bool scientificFormat = false, int maxLineLength = 80);
}  // namespace MatrixModule

#endif  // MATRIX_OPERATIONS_HPP
