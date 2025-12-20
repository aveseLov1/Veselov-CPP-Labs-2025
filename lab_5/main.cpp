#include "cipher_Caesar.hpp"
#include <iostream>
#include <cstdlib>
#include <string_view>

int main(int argc, char* argv[]) {
    std::cout << "Программа шифрования Цезаря\n";
    std::cout << "=============================\n";

    // Аргументы командной строки
    if (argc < 5) {
        std::cerr << "Использование: " << argv[0]
                  << " <исходный_файл> <блокнот> <зашифрованный_файл> <расшифрованный_файл>\n"
                  << "Пример: " << argv[0]
                  << " first.txt code.txt coded.txt decoded.txt\n";
        return EXIT_FAILURE;
    }

    // try {
    //     cipher::Application app;
    //     if (!app.initialize({argv[1], argv[2], argv[3], argv[4]})) {
    //         return EXIT_FAILURE;
    //     }
    //     return app.run() ? EXIT_SUCCESS : EXIT_FAILURE;
    // } catch (const std::exception& e) {
    //     std::cerr << "Ошибка: " << e.what() << "\n";
    //     return EXIT_FAILURE;
    // }
}
