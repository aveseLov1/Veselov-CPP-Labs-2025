#include <stdio.h>
#include <termios.h>
#include <unistd.h>

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
    int in_word = 0;

    // Выводим приглашение
    const char* prompt = "Введите текст: ";
    for (int i = 0; prompt[i] != '\0'; i++) {
        putchar(prompt[i]);
    }

    while (1) {
        char c = getchar();

        // Если точка - завершаем СРАЗУ
        if (c == '.') {
            putchar('.');
            putchar('\n');
            if (in_word) {
                lengths[count - 1] = pos;
            }
            break;
        }

        // Backspace
        if (c == 127 || c == 8) {
            if (in_word && pos > 0) {
                pos--;
                putchar('\b');
                putchar(' ');
                putchar('\b');
            }
            continue;
        }

        // Английские буквы
        if (c >= 'a' && c <= 'z') {
            putchar(c);
            if (!in_word) {
                in_word = 1;
                count++;
                pos = 0;
            }
            words[count - 1][pos] = c;
            pos++;
            continue;
        }

        // Пробел
        if (c == ' ') {
            putchar(' ');
            if (in_word) {
                lengths[count - 1] = pos;
                in_word = 0;
            }
            continue;
        }

        // Все остальные символы игнорируем
    }

    // Восстанавливаем настройки терминала
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    if (count == 0) {
        const char* msg = "Нет слов для обработки\n";
        for (int i = 0; msg[i] != '\0'; i++) {
            putchar(msg[i]);
        }
        return 0;
    }

    // Берем последнее слово
    char last_word[100];
    int last_len = lengths[count - 1];
    for (int i = 0; i < last_len; i++) {
        last_word[i] = words[count - 1][i];
    }

    // Вывод по условию 8
    int first = 1;
    int has_output = 0;

    for (int i = 0; i < count - 1; i++) {
        // Проверяем, отличается ли от последнего
        int different = 0;
        if (lengths[i] != last_len) {
            different = 1;
        } else {
            for (int j = 0; j < lengths[i]; j++) {
                if (words[i][j] != last_word[j]) {
                    different = 1;
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
                    putchar(' ');
                }
                for (int j = 0; j < lengths[i]; j++) {
                    putchar(words[i][j]);
                }
                first = 0;
                has_output = 1;
            }
        }
    }

    if (!has_output) {
        const char* msg = "Нет подходящих слов";
        for (int i = 0; msg[i] != '\0'; i++) {
            putchar(msg[i]);
        }
    }

    putchar('\n');
    return 0;
}
