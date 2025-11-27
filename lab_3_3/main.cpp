#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <stdexcept>

using namespace std;

// ========= ВСТРОЕННЫЙ КЛАСС (тот же, что и в тестах) =========

class cipher_error : public std::exception {
private:
    string message;
public:
    explicit cipher_error(const string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class TableRouteCipher {
private:
    int key;
public:
    TableRouteCipher() = delete;
    TableRouteCipher(int skey) {
        if (skey < 0) {
            throw cipher_error("Ключ должен быть положительным");
        } else if (skey == 0) {
            throw cipher_error("Ключ не может быть нулём");
        }
        key = skey;
    }

    string encrypt(string text) {
        string clean_text;
        for (char c : text) {
            if (c != ' ') clean_text += c;
        }
        if (clean_text.empty())
            throw cipher_error("Отсутствует открытый текст");
        for (char c : clean_text) {
            if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
                throw cipher_error("Некорректные символы в строке");
        }
        if (static_cast<size_t>(key) > clean_text.size()) {
            throw cipher_error("Некорректный размер ключа");
        }

        int length = clean_text.length();
        int rows = (length + key - 1) / key;
        vector<vector<char>> table(rows, vector<char>(key, ' '));
        int index = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < key; j++) {
                if (index < length) {
                    table[i][j] = clean_text[index++];
                }
            }
        }
        string result;
        for (int j = 0; j < key; j++) {
            for (int i = 0; i < rows; i++) {
                if (table[i][j] != ' ') {
                    result += table[i][j];
                }
            }
        }
        return result;
    }

    string decrypt(string text) {
        string clean_text;
        for (char c : text) {
            if (c != ' ') clean_text += c;
        }
        if (clean_text.empty())
            throw cipher_error("Отсутствует открытый текст");
        for (char c : clean_text) {
            if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')))
                throw cipher_error("Некорректные символы в строке");
        }
        if (static_cast<size_t>(key) > clean_text.size()) {
            throw cipher_error("Некорректный размер ключа");
        }

        int length = clean_text.length();
        int rows = (length + key - 1) / key;
        vector<vector<char>> table(rows, vector<char>(key, ' '));
        vector<int> col_counts(key, rows);
        int total_cells = rows * key;
        int empty_cells = total_cells - length;
        for (int j = key - 1; j >= key - empty_cells; j--) {
            col_counts[j] = rows - 1;
        }

        int text_index = 0;
        for (int j = 0; j < key; j++) {
            for (int i = 0; i < col_counts[j]; i++) {
                if (text_index < length) {
                    table[i][j] = clean_text[text_index++];
                }
            }
        }

        string result;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < key; j++) {
                if (table[i][j] != ' ') {
                    result += table[i][j];
                }
            }
        }
        return result;
    }
};

// ========= ОСНОВНАЯ ПРОГРАММА =========

int main() {
    string text;
    int key;
    unsigned vibor;
    
    cout << "--- Шифратор табличным маршрутным шифром ---\n";
    cout << "Поддерживаются только английские буквы и пробелы\n";
    cout << "---\n";
    
    cout << "Введите текст: ";
    getline(cin, text);
    
    cout << "Введите кол-во столбцов: ";
    cin >> key;
    
    try {
        TableRouteCipher shifr(key);
        cout << "Ключ загружен\n";
        
        do {
            cout << "Шифратор готов. Выберите операцию (Выход-0, Шифровка-1, Расшифровка-2): ";
            cin >> vibor;
            
            if (vibor > 2) {
                cout << "Неверная операция!\n";
            } else if (vibor > 0) {
                try {
                    if (vibor == 1) {
                        text = shifr.encrypt(text);
                        cout << "Зашифрованный текст: " << text << endl;
                    } else {
                        text = shifr.decrypt(text);
                        cout << "Расшифрованный текст: " << text << endl;
                    }
                } catch (const cipher_error& e) {
                    cout << "Ошибка при выполнении операции: " << e.what() << endl;
                    break;
                }
            }
        } while (vibor != 0);
        
    } catch (const cipher_error& e) {
        cout << "Ошибка при создании шифратора: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
