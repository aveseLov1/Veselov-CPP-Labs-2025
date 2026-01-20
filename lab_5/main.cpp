#include <cstdlib>
#include <iostream>
#include "cipher_Caesar.hpp"

// КОМАНДА ДЛЯ ЗАПУСКА:
// ./Lab_5 /home/stud.BMSTU/filesCFR/first.txt /home/stud.BMSTU/filesCFR/code.txt /home/stud.BMSTU/filesCFR/coded.txt
// /home/stud.BMSTU/filesCFR/decoded.txt
int main(int argc, char* argv[]) {
    std::cout << "Программа шифрования Цезаря\n";
    std::cout << "=============================\n";

    // Получаем пути к файлам из аргументов командной строки
    cipher::FilePaths paths;
    if (!cipher::parse_arguments(argc, argv, paths)) {
        std::cerr << "\nЗавершение программы из-за ошибки в аргументах\n";
        return EXIT_FAILURE;
    }

    try {
        const bool success = cipher::run_app(paths);

        if (!success) {
            std::cerr << "Программа завершилась с ошибкой\n";
            return EXIT_FAILURE;
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Произошла неизвестная ошибка\n";
        return EXIT_FAILURE;
    }

    std::cout << "\nПрограмма успешно завершена\n";
    return EXIT_SUCCESS;
}
