#include "cipher_Caesar.hpp"
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>

namespace cipher {

// Обертка для безопасного копирования строк
void safe_strcpy(char* dest, const char* src, size_t dest_size) {
    if (dest_size == 0)
        return;

    size_t i = 0;
    while (i < dest_size - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

// Очистка буфера ввода
void clear_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Чтение слов из блокнота с ограничением длины
int read_words(const char* filename, Word words[]) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не могу открыть файл блокнота: " << filename << "\n";
        return 0;
    }

    int count = 0;
    char line[512];  // Увеличил размер буфера

    while (file.getline(line, sizeof(line)) && count < MAX_WORDS) {
        // Если строка пустая - пропускаем
        if (line[0] == '\0')
            continue;

        // Обработка ошибки при чтении длинной строки
        if (file.fail() && !file.eof()) {
            file.clear();
        }

        int len = 0;
        char word_buf[MAX_WORD_LEN] = {0};

        // Извлекаем слова из строки
        for (int i = 0; line[i] != '\0' && len < MAX_WORD_LEN - 1; ++i) {
            char c = line[i];
            if (std::isalnum(static_cast<unsigned char>(c))) {
                // Проверяем ограничение длины кодового слова
                if (len < static_cast<int>(MAX_CODE_WORD_LENGTH)) {
                    word_buf[len++] = c;
                }
                // Если превысили длину - начинаем новое слово
                else if (len == static_cast<int>(MAX_CODE_WORD_LENGTH)) {
                    word_buf[len] = '\0';

                    // Вычисляем ключ
                    int key = 0;
                    for (int j = 0; j < len; ++j) {
                        key += static_cast<unsigned char>(word_buf[j]);
                    }
                    key %= ASCII_SIZE;

                    // Сохраняем слово
                    safe_strcpy(words[count].text, word_buf, MAX_WORD_LEN);
                    words[count].key = key;
                    count++;

                    // Начинаем новое слово с текущего символа
                    len = 0;
                    word_buf[0] = '\0';
                    if (len < static_cast<int>(MAX_CODE_WORD_LENGTH)) {
                        word_buf[len++] = c;
                    }
                }
            } else if (len > 0) {
                word_buf[len] = '\0';

                // Вычисляем ключ
                int key = 0;
                for (int j = 0; j < len; ++j) {
                    key += static_cast<unsigned char>(word_buf[j]);
                }
                key %= ASCII_SIZE;

                // Сохраняем слово
                safe_strcpy(words[count].text, word_buf, MAX_WORD_LEN);
                words[count].key = key;
                count++;

                len = 0;
                word_buf[0] = '\0';
            }
        }

        // Последнее слово в строке
        if (len > 0) {
            word_buf[len] = '\0';
            int key = 0;
            for (int j = 0; j < len; ++j) {
                key += static_cast<unsigned char>(word_buf[j]);
            }
            key %= ASCII_SIZE;

            safe_strcpy(words[count].text, word_buf, MAX_WORD_LEN);
            words[count].key = key;
            count++;
        }
    }

    file.close();

    if (count > 0) {
        std::cout << "Прочитано слов из блокнота: " << count << "\n";
        std::cout << "Максимальная длина слова ограничена: " << MAX_CODE_WORD_LENGTH << " символов\n";
    } else {
        std::cerr << "Ошибка: блокнот пуст или не содержит слов\n";
    }

    return count;
}

// Обработка файла
bool process_file(const char* input_file, const char* output_file, const Word words[], int word_count, bool encode) {
    if (word_count == 0) {
        std::cerr << "Ошибка: Нет слов в блокноте\n";
        return false;
    }

    std::ifstream in(input_file, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Ошибка: Не могу открыть входной файл: " << input_file << "\n";
        return false;
    }

    std::ofstream out(output_file, std::ios::binary | std::ios::trunc);
    if (!out.is_open()) {
        std::cerr << "Ошибка: Не могу открыть выходной файл: " << output_file << "\n";
        in.close();
        return false;
    }

    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    int word_idx = 0;
    int total_chars = 0;

    while (in.read(buffer, BUFFER_SIZE) || in.gcount() > 0) {
        std::streamsize bytes_read = in.gcount();

        // Обрабатываем каждый символ в буфере
        for (std::streamsize i = 0; i < bytes_read; ++i) {
            unsigned char uch = static_cast<unsigned char>(buffer[i]);
            char result_char;

            if (uch < ASCII_SIZE) {
                int shift = words[word_idx].key;

                if (encode) {
                    result_char = static_cast<char>((uch + shift) % ASCII_SIZE);
                } else {
                    result_char = static_cast<char>((uch - shift + ASCII_SIZE) % ASCII_SIZE);
                }

                word_idx = (word_idx + 1) % word_count;
            } else {
                // Не-ASCII символы сохраняем как есть
                result_char = buffer[i];
            }

            out.put(result_char);
            total_chars++;
        }
    }

    in.close();
    out.close();

    if (!out) {
        std::cerr << "Ошибка при записи в файл\n";
        return false;
    }

    std::cout << "Обработано символов: " << total_chars << "\n";
    return true;
}

// Парсинг аргументов командной строки
bool parse_arguments(int argc, char* argv[], FilePaths& paths) {
    // Инициализируем пути пустыми строками
    paths.source[0] = '\0';
    paths.notepad[0] = '\0';
    paths.encoded[0] = '\0';
    paths.decoded[0] = '\0';

    // Проверяем минимальное количество аргументов
    if (argc < 5) {
        std::cout << "Использование:\n";
        std::cout << "  " << argv[0] << " <исходный_файл> <блокнот> <зашифрованный_файл> <расшифрованный_файл>\n";
        std::cout << "(Рекомендуется:)\n";
        std::cout << "  " << argv[0]
                  << " /home/stud.BMSTU/filesCFR/first.txt /home/stud.BMSTU/filesCFR/code.txt /home/stud.BMSTU/filesCFR/coded.txt "
                     "/home/stud.BMSTU/filesCFR/decoded.txt\n";
        return false;
    }

    // Копируем аргументы с проверкой длины
    safe_strcpy(paths.source, argv[1], sizeof(paths.source));
    safe_strcpy(paths.notepad, argv[2], sizeof(paths.notepad));
    safe_strcpy(paths.encoded, argv[3], sizeof(paths.encoded));
    safe_strcpy(paths.decoded, argv[4], sizeof(paths.decoded));

    // Проверяем существование обязательных файлов
    std::ifstream source_check(paths.source, std::ios::binary);
    if (!source_check.is_open()) {
        std::cerr << "Ошибка: Исходный файл '" << paths.source << "' не найден\n";
        return false;
    }
    source_check.close();

    std::ifstream notepad_check(paths.notepad, std::ios::binary);
    if (!notepad_check.is_open()) {
        std::cerr << "Ошибка: Файл блокнота '" << paths.notepad << "' не найден\n";
        return false;
    }
    notepad_check.close();

    std::cout << "\n=== ПУТИ К ФАЙЛАМ ===\n";
    std::cout << "Исходный текст: " << paths.source << "\n";
    std::cout << "Кодовый блокнот: " << paths.notepad << "\n";
    std::cout << "Зашифрованный файл: " << paths.encoded << "\n";
    std::cout << "Расшифрованный файл: " << paths.decoded << "\n";
    std::cout << "Макс. длина слова: " << MAX_CODE_WORD_LENGTH << " символов\n";
    std::cout << "========================\n";

    return true;
}

// Показать меню
MenuOption show_menu() {
    std::cout << "\n=== ШИФР ЦЕЗАРЯ ===\n";
    std::cout << "1. Зашифровать файл\n";
    std::cout << "2. Расшифровать файл\n";
    std::cout << "3. Показать статистику по символу\n";
    std::cout << "4. Сравнить исходный и расшифрованный файлы\n";
    std::cout << "5. Выход\n";
    std::cout << "Выберите действие (1-5): ";

    int choice;
    if (!(std::cin >> choice)) {
        clear_input();
        return MenuOption::EXIT;
    }
    clear_input();  // Очищаем буфер после чтения числа

    if (choice < 1 || choice > 5) {
        return MenuOption::EXIT;
    }

    return static_cast<MenuOption>(choice);
}

// Функция для вывода статистики по символу
bool show_stats(const char* input, const char* notepad, char search_char) {
    Word words[MAX_WORDS];
    int word_count = read_words(notepad, words);

    if (word_count == 0) {
        std::cerr << "Ошибка: Нет слов в блокноте\n";
        return false;
    }

    // Открываем файл для анализа
    std::ifstream file(input, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не могу открыть файл: " << input << "\n";
        return false;
    }

    // Массивы для статистики
    int variant_counts[ASCII_SIZE] = {0};  // Сколько раз символ закодировался в каждый вариант
    int total_encodings = 0;               // Общее количество кодирований символа
    int total_chars = 0;                   // Всего символов в файле
    int total_ascii_chars = 0;             // Всего ASCII символов в файле
    unsigned char search_uchar = static_cast<unsigned char>(search_char);

    // Читаем файл и анализируем
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    int word_idx = 0;

    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
        std::streamsize bytes_read = file.gcount();
        total_chars += bytes_read;

        for (std::streamsize i = 0; i < bytes_read; ++i) {
            unsigned char ch = static_cast<unsigned char>(buffer[i]);

            if (ch < ASCII_SIZE) {
                total_ascii_chars++;

                // Если это искомый символ
                if (ch == search_uchar) {
                    total_encodings++;

                    // Вычисляем, во что он закодируется
                    int shift = words[word_idx].key;
                    unsigned char encoded = (ch + shift) % ASCII_SIZE;

                    // Увеличиваем счетчик для этого варианта кодирования
                    variant_counts[encoded]++;
                }

                // Переходим к следующему слову (ключу)
                word_idx = (word_idx + 1) % word_count;
            }
        }
    }

    file.close();

    // Выводим результаты
    std::cout << "\n=== РЕЗУЛЬТАТЫ АНАЛИЗА ===\n";
    std::cout << "Символ: ";

    if (search_char == '\n') {
        std::cout << "перевод строки (\\n)";
    } else if (search_char == '\t') {
        std::cout << "табуляция (\\t)";
    } else if (search_char == '\r') {
        std::cout << "возврат каретки (\\r)";
    } else if (std::isprint(search_char)) {
        std::cout << "'" << search_char << "'";
    } else {
        std::cout << "[непечатаемый, код " << static_cast<int>(search_uchar) << "]";
    }
    std::cout << "\n";

    std::cout << "ASCII код: " << static_cast<int>(search_uchar) << "\n";
    std::cout << "Всего символов в файле: " << total_chars << "\n";
    std::cout << "ASCII символов в файле: " << total_ascii_chars << "\n";
    std::cout << "Слов в блокноте: " << word_count << "\n";

    if (total_encodings == 0) {
        std::cout << "\nСимвол не найден в исходном файле.\n";
    } else {
        std::cout << "\nСимвол встретился " << total_encodings << " раз.\n";

        // Подсчитываем количество уникальных вариантов кодирования
        int unique_variants = 0;
        for (int i = 0; i < ASCII_SIZE; ++i) {
            if (variant_counts[i] > 0) {
                unique_variants++;
            }
        }

        std::cout << "Количество уникальных вариантов кодирования: " << unique_variants << "\n";

        // Выводим все варианты кодирования
        if (unique_variants > 0) {
            std::cout << "\nВарианты кодирования:\n";
            std::cout << std::setw(10) << "Символ" << std::setw(15) << "ASCII код" << std::setw(20) << "Количество" << std::setw(15) << "Процент"
                      << "\n";
            std::cout << std::string(60, '-') << "\n";

            for (int i = 0; i < ASCII_SIZE; ++i) {
                if (variant_counts[i] > 0) {
                    double percentage = (total_encodings > 0) ? (variant_counts[i] * 100.0 / total_encodings) : 0.0;

                    std::string char_display;
                    char encoded_char = static_cast<char>(i);

                    if (encoded_char == '\n') {
                        char_display = "\\n";
                    } else if (encoded_char == '\t') {
                        char_display = "\\t";
                    } else if (encoded_char == '\r') {
                        char_display = "\\r";
                    } else if (std::isprint(encoded_char)) {
                        char_display = "'";
                        char_display += encoded_char;
                        char_display += "'";
                    } else {
                        char_display = "[код " + std::to_string(i) + "]";
                    }

                    std::cout << std::setw(10) << char_display << std::setw(15) << i << std::setw(20) << variant_counts[i] << std::setw(14)
                              << std::fixed << std::setprecision(2) << percentage << "%\n";
                }
            }
        }
    }

    return true;
}

// Сравнение файлов
bool compare_files(const char* file1, const char* file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1.is_open()) {
        std::cerr << "Ошибка: Не могу открыть файл '" << file1 << "'\n";
        return false;
    }

    if (!f2.is_open()) {
        std::cerr << "Ошибка: Не могу открыть файл '" << file2 << "'\n";
        f1.close();
        return false;
    }

    const int BUFFER_SIZE = 4096;
    char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE];
    bool identical = true;
    long long pos = 0;

    while (f1.read(buf1, BUFFER_SIZE) && f2.read(buf2, BUFFER_SIZE)) {
        if (std::memcmp(buf1, buf2, BUFFER_SIZE) != 0) {
            identical = false;
            break;
        }
        pos += BUFFER_SIZE;
    }

    // Проверяем остатки
    if (identical) {
        std::streamsize count1 = f1.gcount();
        std::streamsize count2 = f2.gcount();

        if (count1 != count2) {
            identical = false;
        } else if (count1 > 0) {
            if (std::memcmp(buf1, buf2, count1) != 0) {
                identical = false;
            }
        }
    }

    f1.close();
    f2.close();

    return identical;
}

// Главный цикл приложения
bool run_app(const FilePaths& paths) {
    bool running = true;
    FilePaths current_paths = paths;

    while (running) {
        MenuOption choice = show_menu();

        switch (choice) {
            case MenuOption::ENCODE: {
                std::cout << "\n=== Шифрование файла ===\n";
                std::cout << "Исходный файл: " << current_paths.source << "\n";
                std::cout << "Файл блокнота: " << current_paths.notepad << "\n";
                std::cout << "Зашифрованный файл: " << current_paths.encoded << "\n";

                if (encode_file(current_paths.source, current_paths.notepad, current_paths.encoded)) {
                    std::cout << "Шифрование завершено успешно\n";
                } else {
                    std::cerr << "Ошибка при шифровании\n";
                }
                break;
            }

            case MenuOption::DECODE: {
                std::cout << "\n=== Дешифрование файла ===\n";
                std::cout << "Зашифрованный файл: " << current_paths.encoded << "\n";
                std::cout << "Файл блокнота: " << current_paths.notepad << "\n";
                std::cout << "Расшифрованный файл: " << current_paths.decoded << "\n";

                if (decode_file(current_paths.encoded, current_paths.notepad, current_paths.decoded)) {
                    std::cout << "Дешифрование завершено успешно\n";
                } else {
                    std::cerr << "Ошибка при дешифровании\n";
                }
                break;
            }

            case MenuOption::STATISTICS: {
                std::cout << "\n=== СТАТИСТИКА ПО СИМВОЛУ ===\n";
                std::cout << "Исходный файл: " << current_paths.source << "\n";
                std::cout << "Файл блокнота: " << current_paths.notepad << "\n";

                // Запрашиваем символ у пользователя
                std::cout << "\nВведите символ для анализа: ";

                char input_buffer[10];
                std::cin.getline(input_buffer, sizeof(input_buffer));

                char search_char;

                // Обработка специальных символов
                if (std::strcmp(input_buffer, "\\n") == 0) {
                    search_char = '\n';
                    std::cout << "Выбран символ: перевод строки (\\n)\n";
                } else if (std::strcmp(input_buffer, "\\t") == 0) {
                    search_char = '\t';
                    std::cout << "Выбран символ: табуляция (\\t)\n";
                } else if (std::strcmp(input_buffer, "\\r") == 0) {
                    search_char = '\r';
                    std::cout << "Выбран символ: возврат каретки (\\r)\n";
                } else if (input_buffer[0] != '\0') {
                    search_char = input_buffer[0];
                    std::cout << "Выбран символ: '" << search_char << "'\n";
                } else {
                    std::cout << "Ошибка: символ не введен\n";
                    break;
                }

                if (!show_stats(current_paths.source, current_paths.notepad, search_char)) {
                    std::cerr << "Ошибка при анализе статистики\n";
                }
                break;
            }

            case MenuOption::COMPARE: {
                std::cout << "\n=== Сравнение файлов ===\n";
                std::cout << "Первый файл: " << current_paths.source << "\n";
                std::cout << "Второй файл: " << current_paths.decoded << "\n";

                if (compare_files(current_paths.source, current_paths.decoded)) {
                    std::cout << "✓ Файлы идентичны\n";
                } else {
                    std::cout << "✗ Файлы различаются\n";
                }
                break;
            }

            case MenuOption::EXIT:
                std::cout << "Выход из программы...\n";
                running = false;
                break;
        }

        if (running) {
            std::cout << "\nНажмите Enter для продолжения...";
            std::cin.get();
        }
    }

    return true;
}

// Оберточные функции для внешнего использования
bool encode_file(const char* input, const char* notepad, const char* output) {
    Word words[MAX_WORDS];
    int word_count = read_words(notepad, words);
    return (word_count > 0) && process_file(input, output, words, word_count, true);
}

bool decode_file(const char* input, const char* notepad, const char* output) {
    Word words[MAX_WORDS];
    int word_count = read_words(notepad, words);
    return (word_count > 0) && process_file(input, output, words, word_count, false);
}

}  // namespace cipher

// #include "cipher_Caesar.hpp"
// #include <iostream>
// #include <fstream>
// #include <iomanip>
// #include <algorithm>
// #include <limits>
// #include <cctype>

// namespace cipher {

// // ================= Notepad Implementation =================
// bool Notepad::load(const std::string& filename) {
//     std::ifstream file(filename);
//     if (!file.is_open()) {
//         std::cerr << "Ошибка: Не могу открыть файл блокнота: " << filename << "\n";
//         return false;
//     }

//     clear();
//     std::string line;
//     std::string current_word;

//     while (std::getline(file, line)) {
//         for (char c : line) {
//             if (std::isalnum(static_cast<unsigned char>(c))) {
//                 if (current_word.length() < MAX_CODE_WORD_LENGTH) {
//                     current_word += c;
//                 } else {
//                     // Сохраняем текущее слово и начинаем новое
//                     if (!current_word.empty()) {
//                         words_.emplace_back(current_word);
//                         current_word.clear();
//                     }
//                     current_word += c;
//                 }
//             } else if (!current_word.empty()) {
//                 words_.emplace_back(current_word);
//                 current_word.clear();
//             }
//         }

//         // Добавляем слово в конце строки
//         if (!current_word.empty()) {
//             words_.emplace_back(current_word);
//             current_word.clear();
//         }
//     }

//     file.close();

//     if (words_.empty()) {
//         std::cerr << "Ошибка: Блокнот не содержит слов\n";
//         return false;
//     }

//     std::cout << "Загружено слов из блокнота: " << words_.size() << "\n";
//     return true;
// }

// // ================= CaesarCipher Implementation =================
// char CaesarCipher::encode(char ch) const {
//     unsigned char uch = static_cast<unsigned char>(ch);
//     if (uch >= ASCII_SIZE || notepad_.size() == 0) {
//         return ch;
//     }

//     const CodeWord& word = notepad_.get_word(current_index_);
//     char result = static_cast<char>((uch + word.key) % ASCII_SIZE);
//     const_cast<CaesarCipher*>(this)->current_index_++;
//     return result;
// }

// char CaesarCipher::decode(char ch) const {
//     unsigned char uch = static_cast<unsigned char>(ch);
//     if (uch >= ASCII_SIZE || notepad_.size() == 0) {
//         return ch;
//     }

//     const CodeWord& word = notepad_.get_word(current_index_);
//     char result = static_cast<char>((uch - word.key + ASCII_SIZE) % ASCII_SIZE);
//     const_cast<CaesarCipher*>(this)->current_index_++;
//     return result;
// }

// // ================= StatisticsAnalyzer Implementation =================
// std::vector<StatisticsAnalyzer::EncodingResult>
// StatisticsAnalyzer::analyze_file(const std::string& filename,
//                                 const Notepad& notepad,
//                                 char target_char) {
//     std::vector<EncodingResult> results;

//     if (notepad.size() == 0) {
//         return results;
//     }

//     std::ifstream file(filename, std::ios::binary);
//     if (!file.is_open()) {
//         throw std::runtime_error("Не могу открыть файл: " + filename);
//     }

//     size_t position = 0;
//     size_t word_index = 0;
//     char ch;

//     while (file.get(ch)) {
//         if (ch == target_char) {
//             unsigned char uch = static_cast<unsigned char>(ch);
//             int key = notepad.get_word(word_index).key;
//             char encoded = static_cast<char>((uch + key) % ASCII_SIZE);

//             results.push_back({ch, encoded, key, position});
//         }

//         if (static_cast<unsigned char>(ch) < ASCII_SIZE) {
//             word_index = (word_index + 1) % notepad.size();
//         }
//         position++;
//     }

//     return results;
// }

// void StatisticsAnalyzer::print_analysis(const std::string& filename,
//                                        const Notepad& notepad,
//                                        char target_char) {
//     auto results = analyze_file(filename, notepad, target_char);

//     std::cout << "\n=== РЕЗУЛЬТАТЫ АНАЛИЗА ===\n";
//     std::cout << "Файл: " << filename << "\n";
//     std::cout << "Символ для анализа: ";

//     if (target_char == '\n') std::cout << "'\\n' (перевод строки)";
//     else if (target_char == '\t') std::cout << "'\\t' (табуляция)";
//     else if (target_char == '\r') std::cout << "'\\r' (возврат каретки)";
//     else if (std::isprint(target_char)) std::cout << "'" << target_char << "'";
//     else std::cout << "[непечатаемый, код " << static_cast<int>(target_char) << "]";

//     std::cout << "\nВсего вхождений: " << results.size() << "\n";

//     if (results.empty()) {
//         std::cout << "Символ не найден в файле.\n";
//         return;
//     }

//     // Группируем по закодированным символам
//     std::vector<std::pair<char, int>> frequency(ASCII_SIZE, {0, 0});
//     for (const auto& res : results) {
//         frequency[static_cast<unsigned char>(res.encoded)] = {res.encoded, 0};
//     }

//     for (const auto& res : results) {
//         frequency[static_cast<unsigned char>(res.encoded)].second++;
//     }

//     // Сортируем по частоте
//     std::sort(frequency.begin(), frequency.end(),
//               [](const auto& a, const auto& b) { return a.second > b.second; });

//     std::cout << "\nВарианты кодирования:\n";
//     std::cout << std::setw(10) << "Символ"
//               << std::setw(15) << "Количество"
//               << std::setw(10) << "Процент\n";
//     std::cout << std::string(35, '-') << "\n";

//     for (const auto& [ch, count] : frequency) {
//         if (count > 0) {
//             double percent = (static_cast<double>(count) / results.size()) * 100.0;
//             std::cout << std::setw(10);

//             if (ch == '\n') std::cout << "\\n";
//             else if (ch == '\t') std::cout << "\\t";
//             else if (ch == '\r') std::cout << "\\r";
//             else if (std::isprint(ch)) std::cout << ch;
//             else std::cout << "[" << static_cast<int>(ch) << "]";

//             std::cout << std::setw(15) << count
//                       << std::setw(10) << std::fixed << std::setprecision(1) << percent << "%\n";
//         }
//     }
// }

// // ================= Application Implementation =================
// bool Application::initialize(const FilePaths& paths) {
//     paths_ = paths;

//     // Проверка существования файлов
//     auto check_file = [](const std::string& filename, const std::string& desc) -> bool {
//         std::ifstream file(filename);
//         if (!file.is_open()) {
//             std::cerr << "Ошибка: " << desc << " файл '" << filename << "' не найден\n";
//             return false;
//         }
//         return true;
//     };

//     if (!check_file(paths_.source, "Исходный")) return false;
//     if (!check_file(paths_.notepad, "Блокнот")) return false;

//     std::cout << "\n=== ПУТИ К ФАЙЛАМ ===\n";
//     std::cout << "Исходный текст: " << paths_.source << "\n";
//     std::cout << "Кодовый блокнот: " << paths_.notepad << "\n";
//     std::cout << "Зашифрованный файл: " << paths_.encoded << "\n";
//     std::cout << "Расшифрованный файл: " << paths_.decoded << "\n";
//     std::cout << "========================\n";

//     return load_notepad();
// }

// bool Application::load_notepad() {
//     return notepad_.load(paths_.notepad);
// }

// void Application::show_menu() const {
//     std::cout << "\n=== ШИФР ЦЕЗАРЯ ===\n";
//     std::cout << "1. Зашифровать файл\n";
//     std::cout << "2. Расшифровать файл\n";
//     std::cout << "3. Показать статистику\n";
//     std::cout << "4. Сравнить файлы\n";
//     std::cout << "5. Выход\n";
//     std::cout << "Выберите действие: ";
// }

// Application::MenuOption Application::get_user_choice() const {
//     int choice;
//     std::cin >> choice;

//     if (std::cin.fail()) {
//         clear_input();
//         return MenuOption::EXIT;
//     }

//     return static_cast<MenuOption>(choice);
// }

// void Application::clear_input() const {
//     std::cin.clear();
//     std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
// }

// bool Application::process_encoding(bool encode) {
//     const std::string& input = encode ? paths_.source : paths_.encoded;
//     const std::string& output = encode ? paths_.encoded : paths_.decoded;
//     const std::string& action = encode ? "шифрования" : "дешифрования";

//     std::cout << "\n=== " << (encode ? "Шифрование" : "Дешифрование") << " файла ===\n";
//     std::cout << "Входной файл: " << input << "\n";
//     std::cout << "Выходной файл: " << output << "\n";

//     CaesarCipher cipher;
//     cipher.set_notepad(notepad_);

//     std::ifstream in_file(input, std::ios::binary);
//     if (!in_file.is_open()) {
//         std::cerr << "Ошибка: Не могу открыть входной файл\n";
//         return false;
//     }

//     std::ofstream out_file(output, std::ios::binary | std::ios::trunc);
//     if (!out_file.is_open()) {
//         std::cerr << "Ошибка: Не могу открыть выходной файл\n";
//         return false;
//     }

//     char ch;
//     size_t count = 0;

//     while (in_file.get(ch)) {
//         char processed = encode ? cipher.encode(ch) : cipher.decode(ch);
//         out_file.put(processed);
//         count++;
//     }

//     in_file.close();
//     out_file.close();

//     std::cout << "Обработано символов: " << count << "\n";
//     std::cout << action << " завершено успешно\n";

//     return true;
// }

// bool Application::compare_files() const {
//     std::cout << "\n=== Сравнение файлов ===\n";
//     std::cout << "Файл 1: " << paths_.source << "\n";
//     std::cout << "Файл 2: " << paths_.decoded << "\n";

//     std::ifstream file1(paths_.source, std::ios::binary);
//     std::ifstream file2(paths_.decoded, std::ios::binary);

//     if (!file1.is_open() || !file2.is_open()) {
//         std::cerr << "Ошибка: Не могу открыть файлы для сравнения\n";
//         return false;
//     }

//     bool identical = true;
//     char c1, c2;
//     size_t line = 1, col = 1;

//     while (file1.get(c1) && file2.get(c2)) {
//         if (c1 != c2) {
//             std::cout << "Различие на строке " << line
//                       << ", позиция " << col << ":\n";
//             std::cout << "  Файл 1: '" << (std::isprint(c1) ? c1 : '?')
//                       << "' (код " << static_cast<int>(c1) << ")\n";
//             std::cout << "  Файл 2: '" << (std::isprint(c2) ? c2 : '?')
//                       << "' (код " << static_cast<int>(c2) << ")\n";
//             identical = false;
//             break;
//         }

//         if (c1 == '\n') {
//             line++;
//             col = 1;
//         } else {
//             col++;
//         }
//     }

//     // Проверка разной длины
//     if (identical && (file1.get(c1) || file2.get(c2))) {
//         std::cout << "Файлы разной длины\n";
//         identical = false;
//     }

//     file1.close();
//     file2.close();

//     std::cout << (identical ? "Файлы идентичны\n" : "Файлы различаются\n");
//     return identical;
// }

// void Application::show_statistics() const {
//     std::cout << "\n=== Анализ статистики ===\n";

//     std::cout << "Введите символ для анализа ";
//     std::cout << "(для специальных символов используйте \\n, \\t, \\r): ";

//     clear_input();
//     std::string input;
//     std::getline(std::cin, input);

//     char target_char;
//     if (input == "\\n") target_char = '\n';
//     else if (input == "\\t") target_char = '\t';
//     else if (input == "\\r") target_char = '\r';
//     else if (!input.empty()) target_char = input[0];
//     else {
//         std::cout << "Ошибка: символ не введен\n";
//         return;
//     }

//     try {
//         StatisticsAnalyzer::print_analysis(paths_.source, notepad_, target_char);
//     } catch (const std::exception& e) {
//         std::cerr << "Ошибка при анализе: " << e.what() << "\n";
//     }
// }

// bool Application::ask_continue() const {
//     std::cout << "\nПродолжить? (y/n): ";
//     char answer;
//     std::cin >> answer;
//     clear_input();
//     return (answer == 'y' || answer == 'Y');
// }

// bool Application::run() {
//     bool running = true;

//     while (running) {
//         show_menu();
//         MenuOption choice = get_user_choice();

//         switch (choice) {
//             case MenuOption::ENCODE:
//                 if (!process_encoding(true)) {
//                     std::cerr << "Ошибка при шифровании\n";
//                 }
//                 break;

//             case MenuOption::DECODE:
//                 if (!process_encoding(false)) {
//                     std::cerr << "Ошибка при дешифровании\n";
//                 }
//                 break;

//             case MenuOption::STATISTICS:
//                 show_statistics();
//                 break;

//             case MenuOption::COMPARE:
//                 compare_files();
//                 break;

//             case MenuOption::EXIT:
//                 std::cout << "Выход из программы...\n";
//                 running = false;
//                 break;

//             default:
//                 std::cout << "Неверный выбор. Попробуйте снова.\n";
//                 clear_input();
//                 continue;
//         }

//         if (running && choice != MenuOption::STATISTICS) {
//             running = ask_continue();
//         }
//     }

//     return true;
// }

// } // namespace cipher
