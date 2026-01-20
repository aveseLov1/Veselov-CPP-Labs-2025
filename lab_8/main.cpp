#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>

using namespace std;

struct Dictionary {
    char* engl;
    char* rus;

    Dictionary() : engl(nullptr), rus(nullptr) {}

    Dictionary(const char* e, const char* r) {
        engl = new char[strlen(e) + 1];
        strcpy(engl, e);

        rus = new char[strlen(r) + 1];
        strcpy(rus, r);
    }

    ~Dictionary() {
        delete[] engl;
        delete[] rus;
    }

    Dictionary(const Dictionary& other) {
        engl = new char[strlen(other.engl) + 1];
        strcpy(engl, other.engl);

        rus = new char[strlen(other.rus) + 1];
        strcpy(rus, other.rus);
    }

    Dictionary& operator=(const Dictionary& other) {
        if (this != &other) {
            delete[] engl;
            delete[] rus;

            engl = new char[strlen(other.engl) + 1];
            strcpy(engl, other.engl);

            rus = new char[strlen(other.rus) + 1];
            strcpy(rus, other.rus);
        }
        return *this;
    }
};

class DictionaryManager {
 private:
    Dictionary* dict;
    int size;
    int capacity;

    void resize() {
        capacity *= 2;
        Dictionary* newDict = new Dictionary[capacity];
        for (int i = 0; i < size; i++) {
            newDict[i] = dict[i];
        }
        delete[] dict;
        dict = newDict;
    }

    int binarySearch(const char* word, int left, int right) {
        while (left <= right) {
            int mid = left + (right - left) / 2;
            int cmp = strcmp(dict[mid].engl, word);

            if (cmp == 0)
                return mid;
            if (cmp < 0)
                left = mid + 1;
            else
                right = mid - 1;
        }
        return -1;
    }

    int findRussianWord(const char* word) {
        for (int i = 0; i < size; i++) {
            if (strcmp(dict[i].rus, word) == 0) {
                return i;
            }
        }
        return -1;
    }

    void sortDictionary() {
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - i - 1; j++) {
                if (strcmp(dict[j].engl, dict[j + 1].engl) > 0) {
                    Dictionary temp = dict[j];
                    dict[j] = dict[j + 1];
                    dict[j + 1] = temp;
                }
            }
        }
    }

    void saveToFile(const char* filename) {
        ofstream file(filename);
        if (!file) {
            cout << "Ошибка открытия файла: " << filename << endl;
            return;
        }

        for (int i = 0; i < size; i++) {
            file << dict[i].engl << endl;
            file << dict[i].rus << endl;
        }
        file.close();
        cout << "Словарь сохранен в файл: " << filename << endl;
    }

    void loadFromFile(const char* filename) {
        ifstream file(filename);
        if (!file) {
            cout << "Файл не найден. Создается новый словарь." << endl;
            initializeDefaultDictionary();
            return;
        }

        clearDictionary();

        char englWord[100];
        char rusWord[100];

        while (file.getline(englWord, 100) && file.getline(rusWord, 100)) {
            addEntry(englWord, rusWord);
        }

        file.close();
        cout << "Словарь загружен из файла: " << filename << endl;
    }

    void initializeDefaultDictionary() {
        addEntry("apple", "яблоко");
        addEntry("book", "книга");
        addEntry("cat", "кот");
        addEntry("dog", "собака");
        addEntry("house", "дом");
        addEntry("sun", "солнце");
        addEntry("moon", "луна");
        addEntry("water", "вода");
        addEntry("tree", "дерево");
        addEntry("friend", "друг");
    }

 public:
    DictionaryManager() : size(0), capacity(10) {
        dict = new Dictionary[capacity];
        loadFromFile("dictionary.txt");
    }

    ~DictionaryManager() {
        saveToFile("dictionary.txt");
        delete[] dict;
    }

    void addEntry(const char* engl, const char* rus) {
        if (size >= capacity) {
            resize();
        }

        dict[size] = Dictionary(engl, rus);
        size++;
        sortDictionary();
        cout << "Добавлено: " << engl << " - " << rus << endl;
    }

    void removeEntry(const char* word) {
        int index = binarySearch(word, 0, size - 1);
        if (index == -1) {
            index = findRussianWord(word);
        }

        if (index == -1) {
            cout << "Слово не найдено: " << word << endl;
            return;
        }

        cout << "Удалено: " << dict[index].engl << " - " << dict[index].rus << endl;

        for (int i = index; i < size - 1; i++) {
            dict[i] = dict[i + 1];
        }
        size--;
    }

    void translateEnglishToRussian(const char* word) {
        int index = binarySearch(word, 0, size - 1);
        if (index == -1) {
            cout << "Перевод не найден для: " << word << endl;
        } else {
            cout << dict[index].engl << " -> " << dict[index].rus << endl;
        }
    }

    void translateRussianToEnglish(const char* word) {
        int index = findRussianWord(word);
        if (index == -1) {
            cout << "Перевод не найден для: " << word << endl;
        } else {
            cout << dict[index].rus << " -> " << dict[index].engl << endl;
        }
    }

    void displayDictionary() {
        if (size == 0) {
            cout << "Словарь пуст." << endl;
            return;
        }

        cout << "\n=== Англо-русский словарь ===\n";
        cout << left << setw(20) << "Английский" << setw(20) << "Русский" << endl;
        cout << string(40, '-') << endl;

        for (int i = 0; i < size; i++) {
            cout << left << setw(20) << dict[i].engl << setw(20) << dict[i].rus << endl;
        }
        cout << "Всего слов: " << size << endl;
    }

    void saveSortedDictionary(const char* filename) {
        sortDictionary();
        saveToFile(filename);
    }

    void clearDictionary() {
        for (int i = 0; i < size; i++) {
            delete[] dict[i].engl;
            delete[] dict[i].rus;
        }
        size = 0;
    }

    void showMenu() {
        int choice;
        char word1[100], word2[100];
        char filename[100];

        do {
            cout << "\n=== Меню словаря ===\n";
            cout << "1. Добавить слово\n";
            cout << "2. Удалить слово\n";
            cout << "3. Перевести с английского на русский\n";
            cout << "4. Перевести с русского на английский\n";
            cout << "5. Просмотреть словарь\n";
            cout << "6. Сохранить отсортированный словарь в файл\n";
            cout << "7. Выйти\n";
            cout << "Выбор: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    cout << "Английское слово: ";
                    cin.getline(word1, 100);
                    cout << "Русское слово: ";
                    cin.getline(word2, 100);
                    addEntry(word1, word2);
                    break;

                case 2:
                    cout << "Слово для удаления (английское или русское): ";
                    cin.getline(word1, 100);
                    removeEntry(word1);
                    break;

                case 3:
                    cout << "Английское слово: ";
                    cin.getline(word1, 100);
                    translateEnglishToRussian(word1);
                    break;

                case 4:
                    cout << "Русское слово: ";
                    cin.getline(word1, 100);
                    translateRussianToEnglish(word1);
                    break;

                case 5:
                    displayDictionary();
                    break;

                case 6:
                    cout << "Имя файла: ";
                    cin.getline(filename, 100);
                    saveSortedDictionary(filename);
                    break;

                case 7:
                    cout << "До свидания!\n";
                    break;

                default:
                    cout << "Неверный выбор. Попробуйте снова.\n";
            }
        } while (choice != 7);
    }
};

int main() {
    DictionaryManager manager;
    manager.showMenu();
    return 0;
}
