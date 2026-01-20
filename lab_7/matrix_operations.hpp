#ifndef MATRIX_OPERATIONS_HPP
#define MATRIX_OPERATIONS_HPP

namespace MatrixModule {

    // Функция выделения памяти для двухмерного динамического массива
    double** allocateMatrix(int numberOfRows, int numberOfColumns);

    // Функция освобождения памяти
    void freeMatrix(double** matrix, int numberOfRows);

    // Функция вычисления факториала
    long long factorial(int n);

    // Функция заполнения матрицы по заданным формулам
    void fillMatrix(double** matrix, int numberOfRows, int numberOfColumns, double x = 1.0);

    // Функция вывода динамической матрицы
    void printDynamicMatrix(double** matrix, int numberOfRows, int numberOfColumns,
                           int precision = 6, bool scientificFormat = false, int maxLineLength = 80);

    // Функция вывода статической матрицы через вспомогательный массив указателей
    void printStaticMatrix(double* rows[], int numberOfRows, int numberOfColumns,
                           int precision = 6, bool scientificFormat = false, int maxLineLength = 80);
}

#endif // MATRIX_OPERATIONS_HPP
