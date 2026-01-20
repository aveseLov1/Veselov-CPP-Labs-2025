#ifndef CIPHER_CAESAR_HPP
#define CIPHER_CAESAR_HPP

#include <cstddef>

namespace cipher {

enum class MenuOption : int {
    ENCODE = 1,
    DECODE = 2,
    STATISTICS = 3,
    COMPARE = 4,
    EXIT = 5
};

constexpr std::size_t MAX_CODE_WORD_LENGTH = 256;
constexpr int ASCII_SIZE = 128;
constexpr int MAX_WORDS = 1000;
constexpr int MAX_WORD_LEN = 256;

struct FilePaths {
    char source[256];
    char notepad[256];
    char encoded[256];
    char decoded[256];
};

struct Word {
    char text[MAX_WORD_LEN];
    int key;
};

// Главные функции
bool run_app(const FilePaths& paths);
MenuOption show_menu();

// Основные операции
bool encode_file(const char* input, const char* notepad, const char* output);
bool decode_file(const char* input, const char* notepad, const char* output);
bool show_stats(const char* input, const char* notepad, char search_char);
bool compare_files(const char* file1, const char* file2);

// Функция для получения путей к файлам из аргументов командной строки
bool parse_arguments(int argc, char* argv[], FilePaths& paths);

// Вспомогательные функции
int read_words(const char* filename, Word words[]);
bool process_file(const char* input_file, const char* output_file, const Word words[], int word_count, bool encode);
void clear_input();

}  // namespace cipher

#endif  // CIPHER_CAESAR_HPP
