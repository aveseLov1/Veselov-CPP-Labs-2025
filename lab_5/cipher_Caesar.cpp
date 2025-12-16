#include "cipher_Caesar.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>

namespace cipher {

// ================= Notepad Implementation =================
bool Notepad::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не могу открыть файл блокнота: " << filename << "\n";
        return false;
    }

    clear();
    std::string line;
    std::string current_word;

    while (std::getline(file, line)) {
        for (char c : line) {
            if (std::isalnum(static_cast<unsigned char>(c))) {
                if (current_word.length() < MAX_CODE_WORD_LENGTH) {
                    current_word += c;
                } else {
                    // Сохраняем текущее слово и начинаем новое
                    if (!current_word.empty()) {
                        words_.emplace_back(current_word);
                        current_word.clear();
                    }
                    current_word += c;
                }
            } else if (!current_word.empty()) {
                words_.emplace_back(current_word);
                current_word.clear();
            }
        }

        // Добавляем слово в конце строки
        if (!current_word.empty()) {
            words_.emplace_back(current_word);
            current_word.clear();
        }
    }

    file.close();

    if (words_.empty()) {
        std::cerr << "Ошибка: Блокнот не содержит слов\n";
        return false;
    }

    std::cout << "Загружено слов из блокнота: " << words_.size() << "\n";
    return true;
}

// ================= CaesarCipher Implementation =================
char CaesarCipher::encode(char ch) const {
    unsigned char uch = static_cast<unsigned char>(ch);
    if (uch >= ASCII_SIZE || notepad_.size() == 0) {
        return ch;
    }

    const CodeWord& word = notepad_.get_word(current_index_);
    char result = static_cast<char>((uch + word.key) % ASCII_SIZE);
    const_cast<CaesarCipher*>(this)->current_index_++;
    return result;
}

char CaesarCipher::decode(char ch) const {
    unsigned char uch = static_cast<unsigned char>(ch);
    if (uch >= ASCII_SIZE || notepad_.size() == 0) {
        return ch;
    }

    const CodeWord& word = notepad_.get_word(current_index_);
    char result = static_cast<char>((uch - word.key + ASCII_SIZE) % ASCII_SIZE);
    const_cast<CaesarCipher*>(this)->current_index_++;
    return result;
}

// ================= StatisticsAnalyzer Implementation =================
std::vector<StatisticsAnalyzer::EncodingResult>
StatisticsAnalyzer::analyze_file(const std::string& filename,
                                const Notepad& notepad,
                                char target_char) {
    std::vector<EncodingResult> results;

    if (notepad.size() == 0) {
        return results;
    }

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Не могу открыть файл: " + filename);
    }

    size_t position = 0;
    size_t word_index = 0;
    char ch;

    while (file.get(ch)) {
        if (ch == target_char) {
            unsigned char uch = static_cast<unsigned char>(ch);
            int key = notepad.get_word(word_index).key;
            char encoded = static_cast<char>((uch + key) % ASCII_SIZE);

            results.push_back({ch, encoded, key, position});
        }

        if (static_cast<unsigned char>(ch) < ASCII_SIZE) {
            word_index = (word_index + 1) % notepad.size();
        }
        position++;
    }

    return results;
}

void StatisticsAnalyzer::print_analysis(const std::string& filename,
                                       const Notepad& notepad,
                                       char target_char) {
    auto results = analyze_file(filename, notepad, target_char);

    std::cout << "\n=== РЕЗУЛЬТАТЫ АНАЛИЗА ===\n";
    std::cout << "Файл: " << filename << "\n";
    std::cout << "Символ для анализа: ";

    if (target_char == '\n') std::cout << "'\\n' (перевод строки)";
    else if (target_char == '\t') std::cout << "'\\t' (табуляция)";
    else if (target_char == '\r') std::cout << "'\\r' (возврат каретки)";
    else if (std::isprint(target_char)) std::cout << "'" << target_char << "'";
    else std::cout << "[непечатаемый, код " << static_cast<int>(target_char) << "]";

    std::cout << "\nВсего вхождений: " << results.size() << "\n";

    if (results.empty()) {
        std::cout << "Символ не найден в файле.\n";
        return;
    }

    // Группируем по закодированным символам
    std::vector<std::pair<char, int>> frequency(ASCII_SIZE, {0, 0});
    for (const auto& res : results) {
        frequency[static_cast<unsigned char>(res.encoded)] = {res.encoded, 0};
    }

    for (const auto& res : results) {
        frequency[static_cast<unsigned char>(res.encoded)].second++;
    }

    // Сортируем по частоте
    std::sort(frequency.begin(), frequency.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });

    std::cout << "\nВарианты кодирования:\n";
    std::cout << std::setw(10) << "Символ"
              << std::setw(15) << "Количество"
              << std::setw(10) << "Процент\n";
    std::cout << std::string(35, '-') << "\n";

    for (const auto& [ch, count] : frequency) {
        if (count > 0) {
            double percent = (static_cast<double>(count) / results.size()) * 100.0;
            std::cout << std::setw(10);

            if (ch == '\n') std::cout << "\\n";
            else if (ch == '\t') std::cout << "\\t";
            else if (ch == '\r') std::cout << "\\r";
            else if (std::isprint(ch)) std::cout << ch;
            else std::cout << "[" << static_cast<int>(ch) << "]";

            std::cout << std::setw(15) << count
                      << std::setw(10) << std::fixed << std::setprecision(1) << percent << "%\n";
        }
    }
}

// ================= Application Implementation =================
bool Application::initialize(const FilePaths& paths) {
    paths_ = paths;

    // Проверка существования файлов
    auto check_file = [](const std::string& filename, const std::string& desc) -> bool {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Ошибка: " << desc << " файл '" << filename << "' не найден\n";
            return false;
        }
        return true;
    };

    if (!check_file(paths_.source, "Исходный")) return false;
    if (!check_file(paths_.notepad, "Блокнот")) return false;

    std::cout << "\n=== ПУТИ К ФАЙЛАМ ===\n";
    std::cout << "Исходный текст: " << paths_.source << "\n";
    std::cout << "Кодовый блокнот: " << paths_.notepad << "\n";
    std::cout << "Зашифрованный файл: " << paths_.encoded << "\n";
    std::cout << "Расшифрованный файл: " << paths_.decoded << "\n";
    std::cout << "========================\n";

    return load_notepad();
}

bool Application::load_notepad() {
    return notepad_.load(paths_.notepad);
}

void Application::show_menu() const {
    std::cout << "\n=== ШИФР ЦЕЗАРЯ ===\n";
    std::cout << "1. Зашифровать файл\n";
    std::cout << "2. Расшифровать файл\n";
    std::cout << "3. Показать статистику\n";
    std::cout << "4. Сравнить файлы\n";
    std::cout << "5. Выход\n";
    std::cout << "Выберите действие: ";
}

Application::MenuOption Application::get_user_choice() const {
    int choice;
    std::cin >> choice;

    if (std::cin.fail()) {
        clear_input();
        return MenuOption::EXIT;
    }

    return static_cast<MenuOption>(choice);
}

void Application::clear_input() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

bool Application::process_encoding(bool encode) {
    const std::string& input = encode ? paths_.source : paths_.encoded;
    const std::string& output = encode ? paths_.encoded : paths_.decoded;
    const std::string& action = encode ? "шифрования" : "дешифрования";

    std::cout << "\n=== " << (encode ? "Шифрование" : "Дешифрование") << " файла ===\n";
    std::cout << "Входной файл: " << input << "\n";
    std::cout << "Выходной файл: " << output << "\n";

    CaesarCipher cipher;
    cipher.set_notepad(notepad_);

    std::ifstream in_file(input, std::ios::binary);
    if (!in_file.is_open()) {
        std::cerr << "Ошибка: Не могу открыть входной файл\n";
        return false;
    }

    std::ofstream out_file(output, std::ios::binary | std::ios::trunc);
    if (!out_file.is_open()) {
        std::cerr << "Ошибка: Не могу открыть выходной файл\n";
        return false;
    }

    char ch;
    size_t count = 0;

    while (in_file.get(ch)) {
        char processed = encode ? cipher.encode(ch) : cipher.decode(ch);
        out_file.put(processed);
        count++;
    }

    in_file.close();
    out_file.close();

    std::cout << "Обработано символов: " << count << "\n";
    std::cout << action << " завершено успешно\n";

    return true;
}

bool Application::compare_files() const {
    std::cout << "\n=== Сравнение файлов ===\n";
    std::cout << "Файл 1: " << paths_.source << "\n";
    std::cout << "Файл 2: " << paths_.decoded << "\n";

    std::ifstream file1(paths_.source, std::ios::binary);
    std::ifstream file2(paths_.decoded, std::ios::binary);

    if (!file1.is_open() || !file2.is_open()) {
        std::cerr << "Ошибка: Не могу открыть файлы для сравнения\n";
        return false;
    }

    bool identical = true;
    char c1, c2;
    size_t line = 1, col = 1;

    while (file1.get(c1) && file2.get(c2)) {
        if (c1 != c2) {
            std::cout << "Различие на строке " << line
                      << ", позиция " << col << ":\n";
            std::cout << "  Файл 1: '" << (std::isprint(c1) ? c1 : '?')
                      << "' (код " << static_cast<int>(c1) << ")\n";
            std::cout << "  Файл 2: '" << (std::isprint(c2) ? c2 : '?')
                      << "' (код " << static_cast<int>(c2) << ")\n";
            identical = false;
            break;
        }

        if (c1 == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
    }

    // Проверка разной длины
    if (identical && (file1.get(c1) || file2.get(c2))) {
        std::cout << "Файлы разной длины\n";
        identical = false;
    }

    file1.close();
    file2.close();

    std::cout << (identical ? "Файлы идентичны\n" : "Файлы различаются\n");
    return identical;
}

void Application::show_statistics() const {
    std::cout << "\n=== Анализ статистики ===\n";

    std::cout << "Введите символ для анализа ";
    std::cout << "(для специальных символов используйте \\n, \\t, \\r): ";

    clear_input();
    std::string input;
    std::getline(std::cin, input);

    char target_char;
    if (input == "\\n") target_char = '\n';
    else if (input == "\\t") target_char = '\t';
    else if (input == "\\r") target_char = '\r';
    else if (!input.empty()) target_char = input[0];
    else {
        std::cout << "Ошибка: символ не введен\n";
        return;
    }

    try {
        StatisticsAnalyzer::print_analysis(paths_.source, notepad_, target_char);
    } catch (const std::exception& e) {
        std::cerr << "Ошибка при анализе: " << e.what() << "\n";
    }
}

bool Application::ask_continue() const {
    std::cout << "\nПродолжить? (y/n): ";
    char answer;
    std::cin >> answer;
    clear_input();
    return (answer == 'y' || answer == 'Y');
}

bool Application::run() {
    bool running = true;

    while (running) {
        show_menu();
        MenuOption choice = get_user_choice();

        switch (choice) {
            case MenuOption::ENCODE:
                if (!process_encoding(true)) {
                    std::cerr << "Ошибка при шифровании\n";
                }
                break;

            case MenuOption::DECODE:
                if (!process_encoding(false)) {
                    std::cerr << "Ошибка при дешифровании\n";
                }
                break;

            case MenuOption::STATISTICS:
                show_statistics();
                break;

            case MenuOption::COMPARE:
                compare_files();
                break;

            case MenuOption::EXIT:
                std::cout << "Выход из программы...\n";
                running = false;
                break;

            default:
                std::cout << "Неверный выбор. Попробуйте снова.\n";
                clear_input();
                continue;
        }

        if (running && choice != MenuOption::STATISTICS) {
            running = ask_continue();
        }
    }

    return true;
}

} // namespace cipher
