#ifndef CIPHER_HPP
#define CIPHER_HPP

#include <string>
#include <vector>
#include <memory>
#include <cstddef>

namespace cipher {

// Константы
constexpr std::size_t MAX_CODE_WORD_LENGTH = 6;
constexpr int ASCII_SIZE = 128;
constexpr int MAX_WORDS = 1000;

// Структура для кодового слова
struct CodeWord {
    std::string text;
    int key;

    CodeWord(const std::string& word) : text(word), key(0) {
        for (char c : text) {
            key += static_cast<unsigned char>(c);
        }
        key %= ASCII_SIZE;
    }
};

// Класс для работы с блокнотом
class Notepad {
private:
    std::vector<CodeWord> words_;

public:
    bool load(const std::string& filename);
    size_t size() const { return words_.size(); }
    const CodeWord& get_word(size_t index) const { return words_[index % words_.size()]; }
    void clear() { words_.clear(); }

    const std::vector<CodeWord>& words() const { return words_; }
};

// Класс для шифрования/дешифрования
class CaesarCipher {
private:
    Notepad notepad_;
    size_t current_index_;

public:
    CaesarCipher() : current_index_(0) {}

    void set_notepad(const Notepad& notepad) {
        notepad_ = notepad;
        current_index_ = 0;
    }

    char encode(char ch) const;
    char decode(char ch) const;

    void reset() { current_index_ = 0; }
};

// Класс для анализа статистики
class StatisticsAnalyzer {
public:
    struct EncodingResult {
        char original;
        char encoded;
        int key;
        size_t position;
    };

    static std::vector<EncodingResult> analyze_file(
        const std::string& filename,
        const Notepad& notepad,
        char target_char
    );

    static void print_analysis(
        const std::string& filename,
        const Notepad& notepad,
        char target_char
    );
};

// Класс приложения
class Application {
private:
    struct FilePaths {
        std::string source;
        std::string notepad;
        std::string encoded;
        std::string decoded;
    };

    FilePaths paths_;
    Notepad notepad_;

    enum class MenuOption {
        ENCODE = 1,
        DECODE = 2,
        STATISTICS = 3,
        COMPARE = 4,
        EXIT = 5
    };

    void show_menu() const;
    MenuOption get_user_choice() const;
    void clear_input() const;

    bool load_notepad();
    bool process_encoding(bool encode);
    bool compare_files() const;
    void show_statistics() const;
    bool ask_continue() const;

public:
    bool initialize(const FilePaths& paths);
    bool run();
};

} // namespace cipher

#endif // CIPHER_HPP
