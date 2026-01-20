#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

class Matrix {
 private:
    vector<vector<double>> data;
    int n;

 public:
    Matrix(int size) : n(size) { data.resize(n, vector<double>(n, 0.0)); }

    Matrix(const vector<vector<double>>& values) {
        n = values.size();
        data = values;
    }

    // Получение элемента матрицы
    double& operator()(int i, int j) { return data[i][j]; }

    double operator()(int i, int j) const { return data[i][j]; }

    // Получение размера матрицы
    int size() const { return n; }

    // Вывод матрицы
    void print(const string& title = "") const {
        if (!title.empty()) {
            cout << title << ":" << endl;
        }
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << setw(12) << fixed << setprecision(6) << data[i][j];
            }
            cout << endl;
        }
        cout << endl;
    }

    // Проверка на равенство с единичной матрицей
    bool isIdentity(double tolerance = 1e-6) const {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    if (fabs(data[i][j] - 1.0) > tolerance)
                        return false;
                } else {
                    if (fabs(data[i][j]) > tolerance)
                        return false;
                }
            }
        }
        return true;
    }
};

// Функция для вычисления обратной матрицы методом Гаусса-Жордана
Matrix inverseGaussJordan(const Matrix& A) {
    int n = A.size();

    // Создаем расширенную матрицу [A|I]
    vector<vector<double>> augmented(n, vector<double>(2 * n, 0.0));

    // Заполняем левую часть исходной матрицей A
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            augmented[i][j] = A(i, j);
        }
        // Правая часть - единичная матрица
        augmented[i][i + n] = 1.0;
    }

    // Прямой ход метода Гаусса-Жордана
    for (int k = 0; k < n; k++) {
        // Поиск максимального элемента в столбце k (частичный выбор главного элемента)
        int maxRow = k;
        double maxVal = fabs(augmented[k][k]);

        for (int i = k + 1; i < n; i++) {
            if (fabs(augmented[i][k]) > maxVal) {
                maxVal = fabs(augmented[i][k]);
                maxRow = i;
            }
        }

        // Если максимальный элемент равен 0, матрица вырожденная
        if (fabs(augmented[maxRow][k]) < 1e-12) {
            throw runtime_error("Матрица вырожденная, обратной не существует");
        }

        // Перестановка строк, если нужно
        if (maxRow != k) {
            swap(augmented[k], augmented[maxRow]);
        }

        // Нормировка k-й строки
        double pivot = augmented[k][k];
        for (int j = 0; j < 2 * n; j++) {
            augmented[k][j] /= pivot;
        }

        // Исключение элементов в столбце k из других строк
        for (int i = 0; i < n; i++) {
            if (i != k) {
                double factor = augmented[i][k];
                for (int j = 0; j < 2 * n; j++) {
                    augmented[i][j] -= factor * augmented[k][j];
                }
            }
        }
    }

    // Извлечение обратной матрицы из правой части расширенной матрицы
    Matrix inv(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inv(i, j) = augmented[i][j + n];
        }
    }

    return inv;
}

// Умножение матриц
Matrix multiplyMatrices(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix result(n);

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result(i, j) = 0.0;
            for (int k = 0; k < n; k++) {
                result(i, j) += A(i, k) * B(k, j);
            }
        }
    }

    return result;
}

// Тестирование на примере из описания
void testExample() {
    cout << "Тестирование на примере из описания:" << endl;
    cout << "=====================================" << endl;

    // Исходная матрица из примера
    Matrix A({
        {2, 5,  7 },
        {3, 9,  15},
        {5, 16, 20}
    });

    A.print("Исходная матрица A");

    try {
        // Вычисление обратной матрицы
        Matrix A_inv = inverseGaussJordan(A);
        A_inv.print("Обратная матрица A^(-1)");

        // Проверка: умножение исходной матрицы на обратную
        Matrix I = multiplyMatrices(A, A_inv);
        I.print("Проверка: A * A^(-1) (должна быть единичная матрица)");

        // Проверка точности
        if (I.isIdentity(1e-6)) {
            cout << "Проверка пройдена: получена единичная матрица" << endl;
        } else {
            cout << "Внимание: проверка не пройдена - результат не является единичной матрицей" << endl;
        }

    } catch (const runtime_error& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}

// Демонстрация работы с пользовательской матрицей
void userExample() {
    cout << "\nДемонстрация работы с пользовательской матрицей:" << endl;
    cout << "==============================================" << endl;

    int n;
    cout << "Введите размер матрицы: ";
    cin >> n;

    Matrix A(n);

    cout << "Введите элементы матрицы построчно:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << "A[" << i << "][" << j << "] = ";
            cin >> A(i, j);
        }
    }

    A.print("\nВведенная матрица A");

    try {
        // Вычисление обратной матрицы
        Matrix A_inv = inverseGaussJordan(A);
        A_inv.print("Обратная матрица A^(-1)");

        // Проверка
        Matrix I = multiplyMatrices(A, A_inv);
        I.print("Проверка: A * A^(-1)");

        if (I.isIdentity(1e-6)) {
            cout << "Проверка пройдена: получена единичная матрица" << endl;
        } else {
            cout << "Внимание: проверка не пройдена" << endl;
        }

    } catch (const runtime_error& e) {
        cout << "Ошибка: " << e.what() << endl;
    }
}

int main() {
    // Установка локали для корректного отображения чисел
    setlocale(LC_ALL, "Russian");

    cout << "Вычисление обратной матрицы методом Гаусса-Жордана" << endl;
    cout << "==================================================" << endl;

    int choice;
    do {
        cout << "\nМеню:" << endl;
        cout << "1. Тестирование на примере из описания" << endl;
        cout << "2. Ввод своей матрицы" << endl;
        cout << "3. Пример вырожденной матрицы" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
            case 1:
                testExample();
                break;

            case 2:
                userExample();
                break;

            case 3: {
                cout << "\nПример вырожденной матрицы:" << endl;
                cout << "============================" << endl;
                Matrix singular({
                    {1, 2, 3},
                    {4, 5, 6},
                    {7, 8, 9}
                });
                singular.print("Вырожденная матрица (определитель = 0)");

                try {
                    Matrix inv = inverseGaussJordan(singular);
                    inv.print("Обратная матрица");
                } catch (const runtime_error& e) {
                    cout << "Ожидаемая ошибка: " << e.what() << endl;
                }
                break;
            }

            case 0:
                cout << "Выход из программы" << endl;
                break;

            default:
                cout << "Неверный выбор, попробуйте снова" << endl;
        }
    } while (choice != 0);

    return 0;
}
