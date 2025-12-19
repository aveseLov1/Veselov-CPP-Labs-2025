#include <termios.h>
#include <unistd.h>
#include <iostream>

int main() {
    // Сохраняем текущие настройки терминала
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Отключаем канонический режим (буферизацию) и эхо
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    char words[1000][100];
    int lengths[1000];
    int count = 0;
    int pos = 0;
    bool in_word = false;

    std::cout << "Введите текст: ";

    while (true) {
        char c = std::cin.get();

        // Если точка - завершаем СРАЗУ
        if (c == '.') {
            std::cout.put('.');   // Выводим точку на экран
            std::cout.put('\n');  // Переход на новую строку
            if (in_word) {
                lengths[count - 1] = pos;
            }
            break;
        }

        // Backspace (ASCII 127 в Linux/Mac, 8 в некоторых системах)
        if (c == 127 || c == 8) {
            if (in_word && pos > 0) {
                pos--;
                // В небуферизованном режиме нужно стереть символ с экрана
                std::cout.put('\b');
                std::cout.put(' ');
                std::cout.put('\b');
            }
            continue;
        }

        // Английские буквы
        if (c >= 'a' && c <= 'z') {
            std::cout.put(c);  // Выводим букву на экран
            if (!in_word) {
                in_word = true;
                count++;
                pos = 0;
            }
            words[count - 1][pos] = c;
            pos++;
            continue;
        }

        // Пробел
        if (c == ' ') {
            std::cout.put(' ');  // Выводим пробел на экран
            if (in_word) {
                lengths[count - 1] = pos;
                in_word = false;
            }
            continue;
        }

        // Все остальные символы (цифры, русские буквы и т.д.) НЕ ВЫВОДИМ
        // Просто игнорируем
    }

    // Восстанавливаем настройки терминала
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (count == 0) {
        std::cout << "Нет слов для обработки\n";
        return 0;
    }

    // Берем последнее слово
    char last_word[100];
    int last_len = lengths[count - 1];
    for (int i = 0; i < last_len; i++) {
        last_word[i] = words[count - 1][i];
    }

    // Вывод по условию 8
    bool first = true;
    bool has_output = false;

    for (int i = 0; i < count - 1; i++) {
        // Проверяем, отличается ли от последнего
        bool different = false;
        if (lengths[i] != last_len) {
            different = true;
        } else {
            for (int j = 0; j < lengths[i]; j++) {
                if (words[i][j] != last_word[j]) {
                    different = true;
                    break;
                }
            }
        }

        if (different) {
            // Проверяем условие: первая буква встречается только 1 раз
            char first_letter = words[i][0];
            int occurrences = 0;
            for (int j = 0; j < lengths[i]; j++) {
                if (words[i][j] == first_letter) {
                    occurrences++;
                }
            }

            if (occurrences == 1) {
                if (!first) {
                    std::cout.put(' ');
                }
                for (int j = 0; j < lengths[i]; j++) {
                    std::cout.put(words[i][j]);
                }
                first = false;
                has_output = true;
            }
        }
    }

    if (!has_output) {
        std::cout << "Нет подходящих слов";
    }

    std::cout.put('\n');
    return 0;
}
