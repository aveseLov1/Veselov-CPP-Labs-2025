#include "matrix_operations.hpp"
#include <iostream>
#include <limits>

int main() {
  // Часть первая: Работа с динамической матрицей
  std::cout << "========================================\n";
  std::cout << "ЧАСТЬ ПЕРВАЯ: Динамическая матрица\n";
  std::cout << "========================================\n\n";

  std::size_t numberOfRows = 0;
  std::size_t numberOfColumns = 0;

  // Ввод размеров матрицы
  std::cout << "Введите количество строк и столбцов матрицы: ";
  std::cin >> numberOfRows >> numberOfColumns;

  // Проверка ввода
  if (std::cin.fail() || numberOfRows == 0 || numberOfColumns == 0) {
    std::cerr << "Ошибка: некорректные размеры матрицы. "
              << "Должны быть положительные числа.\n";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return 1;
  }

  // Создание динамической матрицы
  double **twoDimensionalDynamicMatrix = nullptr;
  try {
    twoDimensionalDynamicMatrix =
        MatrixModule::allocateMatrix(numberOfRows, numberOfColumns);
  } catch (const std::bad_alloc &e) {
    std::cerr << "Ошибка выделения памяти: " << e.what() << "\n";
    return 1;
  }

  // Заполнение матрицы
  MatrixModule::fillMatrix(twoDimensionalDynamicMatrix, numberOfRows,
                           numberOfColumns);

  // Вывод матрицы в разных форматах
  std::cout << "\nМатрица в фиксированном формате (точность 4):\n";
  MatrixModule::printDynamicMatrix(twoDimensionalDynamicMatrix, numberOfRows,
                                   numberOfColumns, 4, false, 80);

  std::cout << "\n\nМатрица в научном формате (точность 6):\n";
  MatrixModule::printDynamicMatrix(twoDimensionalDynamicMatrix, numberOfRows,
                                   numberOfColumns, 6, true, 80);

  // Часть вторая: Работа со статической матрицей
  std::cout << "\n\n========================================\n";
  std::cout << "ЧАСТЬ ВТОРАЯ: Статическая матрица\n";
  std::cout << "========================================\n\n";

  // Создание и заполнение статической матрицы
  const std::size_t STATIC_SIZE = 10;
  double staticMatrix[STATIC_SIZE][STATIC_SIZE];

  for (std::size_t i = 0; i < STATIC_SIZE; ++i) {
    for (std::size_t j = 0; j < STATIC_SIZE; ++j) {
      staticMatrix[i][j] = static_cast<double>(i) * 10.0 + static_cast<double>(j);
    }
  }

  // Создание вспомогательного массива указателей на строки
  const double *pointerArray[STATIC_SIZE];
  for (std::size_t i = 0; i < STATIC_SIZE; ++i) {
    pointerArray[i] = staticMatrix[i];
  }

  // Вывод статической матрицы через функцию печати
  std::cout << "Статическая матрица 10x10 (aij = i*10 + j):\n";
  MatrixModule::printStaticMatrix(pointerArray, STATIC_SIZE, STATIC_SIZE, 2,
                                  false, 80);

  // Объяснение передачи матриц в функцию
  std::cout << "\n\nОбъяснение передачи матриц в функцию:\n";
  std::cout << "1. Динамическая матрица передается как double** - указатель на "
               "указатель,\n";
  std::cout << "   так как это массив указателей на строки.\n";
  std::cout << "2. Статическая матрица передается через массив указателей: "
               "double* rows[],\n";
  std::cout << "   где каждый элемент указывает на строку статической "
               "матрицы.\n";
  std::cout << "3. В обоих случаях функция получает доступ к элементам через "
               "двойную индексацию.\n";

  // Демонстрация работы с указателями
  std::cout << "\n\nДемонстрация работы с указателями для "
               "staticMatrix[10][10]:\n";
  std::cout << "===========================================================\n\n";

  // Создаем псевдоним для удобства
  auto B = staticMatrix;

  std::cout << "1. std::cout << staticMatrix << \"  \" << staticMatrix[0] "
               "<< \"  \" << staticMatrix[2] << std::endl;\n";
  std::cout << "   Результат: " << staticMatrix << "  " << staticMatrix[0]
            << "  " << staticMatrix[2] << std::endl;
  std::cout << "   Объяснение:\n";
  std::cout << "   - staticMatrix: адрес начала массива (указатель на первую "
               "строку)\n";
  std::cout << "   - staticMatrix[0]: адрес первого элемента первой строки "
               "(эквивалентно &staticMatrix[0][0])\n";
  std::cout << "   - staticMatrix[2]: адрес первого элемента третьей строки\n\n";

  std::cout << "2. std::cout << staticMatrix[0][0] << \"  \" << "
               "**staticMatrix << \"  \" << *staticMatrix[0] << std::endl;\n";
  std::cout << "   Результат: " << staticMatrix[0][0] << "  " << **staticMatrix
            << "  " << *staticMatrix[0] << std::endl;
  std::cout << "   Объяснение:\n";
  std::cout << "   - staticMatrix[0][0]: значение элемента в первой строке, "
               "первом столбце (0)\n";
  std::cout << "   - **staticMatrix: двойное разыменование дает тот же "
               "элемент (0)\n";
  std::cout << "   - *staticMatrix[0]: разыменование указателя на первую "
               "строку дает первый элемент (0)\n\n";

  std::cout << "3. std::cout << *(*(staticMatrix+1)) << \"  \" << *B[1] "
               "<< std::endl;\n";
  std::cout << "   Результат: " << *(*(staticMatrix + 1)) << "  " << *B[1]
            << std::endl;
  std::cout << "   Объяснение:\n";
  std::cout << "   - *(*(staticMatrix+1)): staticMatrix+1 - указатель на "
               "вторую строку,\n";
  std::cout << "     разыменование дает адрес первого элемента второй "
               "строки,\n";
  std::cout << "     второе разыменование дает значение staticMatrix[1][0] "
               "(10)\n";
  std::cout << "   - *B[1]: B[1] - адрес первой строки, разыменование дает "
               "B[1][0] (10)\n\n";

  std::cout << "4. std::cout << *(staticMatrix[0]+1) << \"  \" << "
               "*(*staticMatrix+1) << std::endl;\n";
  std::cout << "   Результат: " << *(staticMatrix[0] + 1) << "  "
            << *(*staticMatrix + 1) << std::endl;
  std::cout << "   Объяснение:\n";
  std::cout << "   - *(staticMatrix[0]+1): staticMatrix[0]+1 - адрес второго "
               "элемента первой строки,\n";
  std::cout << "     разыменование дает staticMatrix[0][1] (1)\n";
  std::cout << "   - *(*staticMatrix+1): *staticMatrix - адрес первого "
               "элемента,\n";
  std::cout << "     +1 смещает на один элемент, разыменование дает тот же "
               "элемент (1)\n\n";

  std::cout << "5. std::cout << staticMatrix[0][20] << \" \" << "
               "*(staticMatrix[0]+20) << \"  \" << *staticMatrix[2] << std::endl;\n";
  std::cout << "   ВНИМАНИЕ: Этот код содержит неопределенное поведение!\n";
  std::cout << "   staticMatrix[0][20] и *(staticMatrix[0]+20) - обращение за "
               "границы массива.\n";
  std::cout << "   Теоретически, если бы это было разрешено, результат мог бы "
               "быть:\n";
  std::cout << "   Результат: 20 20  20\n";
  std::cout << "   Объяснение:\n";
  std::cout << "   - staticMatrix[0][20]: выход за границы строки (размер: 10 "
               "элементов),\n";
  std::cout << "     но так как матрица хранится по строкам непрерывно,\n";
  std::cout << "     это могло бы обратиться к staticMatrix[2][0] = 20\n";
  std::cout << "   - *(staticMatrix[0]+20): аналогичный доступ через "
               "указатели\n";
  std::cout << "   - *staticMatrix[2]: легальный доступ к staticMatrix[2][0] = "
               "20\n";
  std::cout << "   В реальном коде НИКОГДА не используйте доступ за границы "
               "массивов!\n";

  // Освобождение памяти динамической матрицы
  MatrixModule::freeMatrix(twoDimensionalDynamicMatrix, numberOfRows);

  return 0;
}
