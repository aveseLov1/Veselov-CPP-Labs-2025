#include <iostream>
#include <termios.h>
#include <unistd.h>

int main() {
    // Сохраняем текущие настройки терминала
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Отключаем канонический режим и эхо
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char words[1000][100];
    int lengths[1000];
    int word_count = 0;
    int pos = 0;
    bool in_word = false;

    // Выводим приглашение
    const char* prompt = "Введите текст: ";
    for (int i = 0; prompt[i] != '\0'; ++i) {
        std::cout.put(prompt[i]);
    }

    while (true) {
        char c = std::cin.get();

        // Если точка - завершаем ввод
        if (c == '.') {
            std::cout.put('.');
            std::cout.put('\n');
            if (in_word) {
                lengths[word_count - 1] = pos;
            }
            break;
        }

        // Backspace
        if (c == 127 || c == 8) {
            // Всегда пытаемся стереть символ на экране
            std::cout.put('\b');
            std::cout.put(' ');
            std::cout.put('\b');

            // Обновляем внутренние структуры данных
            if (in_word && pos > 0) {
                // Удаляем букву из текущего слова
                --pos;

                // Если слово стало пустым
                if (pos == 0) {
                    in_word = false;
                    // Уменьшаем счетчик только если слово было не пустое изначально
                    if (word_count > 0) {
                        --word_count;
                    }
                }
            }
            // Если мы между словами (после пробела)
            else if (!in_word && word_count > 0) {
                // Возвращаемся к предыдущему слову
                in_word = true;
                pos = lengths[word_count - 1];
                // Здесь НЕ уменьшаем word_count, так как слово уже существует
            }
            // Если мы в начале и нет слов, просто игнорируем
            // (уже стерли символ на экране выше)
            continue;
        }

        // Английские буквы
        if (c >= 'a' && c <= 'z') {
            std::cout.put(c);

            if (!in_word) {
                in_word = true;
                ++word_count;
                pos = 0;
            }

            // Сохраняем букву
            if (pos < 100) {
                words[word_count - 1][pos] = c;
                ++pos;
            }
            continue;
        }

        // Пробел
        if (c == ' ') {
            std::cout.put(' ');

            if (in_word) {
                lengths[word_count - 1] = pos;
                in_word = false;
            }
            continue;
        }

        // Остальные символы игнорируем
    }

    // Восстанавливаем настройки терминала
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (word_count == 0) {
        const char* msg = "Нет слов для обработки\n";
        for (int i = 0; msg[i] != '\0'; ++i) {
            std::cout.put(msg[i]);
        }
        return 0;
    }

    // Сохраняем последнее слово
    char last_word[100];
    int last_len = lengths[word_count - 1];
    for (int i = 0; i < last_len; ++i) {
        last_word[i] = words[word_count - 1][i];
    }

    // Вывод результата
    bool first_output = true;
    bool has_output = false;

    for (int i = 0; i < word_count - 1; ++i) {
        // Проверяем, отличается ли от последнего
        bool different = false;

        if (lengths[i] != last_len) {
            different = true;
        } else {
            for (int j = 0; j < lengths[i]; ++j) {
                if (words[i][j] != last_word[j]) {
                    different = true;
                    break;
                }
            }
        }

        if (different) {
            // Проверяем условие: первая буква встречается ровно 2 раза
            char first_letter = words[i][0];
            int occurrences = 0;

            for (int j = 0; j < lengths[i]; ++j) {
                if (words[i][j] == first_letter) {
                    ++occurrences;
                }
            }

            if (occurrences == 2) {
                if (!first_output) {
                    std::cout.put(' ');
                }

                for (int j = 0; j < lengths[i]; ++j) {
                    std::cout.put(words[i][j]);
                }

                first_output = false;
                has_output = true;
            }
        }
    }

    if (!has_output) {
        const char* msg = "Нет подходящих слов";
        for (int i = 0; msg[i] != '\0'; ++i) {
            std::cout.put(msg[i]);
        }
    }

    std::cout.put('\n');
    return 0;
}
