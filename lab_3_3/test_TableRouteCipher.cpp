#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <functional>
#include <cctype>

using namespace std;

// ========= ВСТРОЕННЫЙ КЛАСС TableRouteCipher =========

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
        // Ключ = 1 теперь РАЗРЕШЁН
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
        // Разрешаем key >= 1, но запрещаем key > длина текста (кроме пустого)
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

// ========= ФУНКЦИИ ДЛЯ ТЕСТИРОВАНИЯ =========

bool expect_exception(const string& test_name, const function<void()>& func) {
    try {
        func();
        cout << "Тест " << test_name << "... НЕ ПРОЙДЕН: ожидалось исключение\n";
        return false;
    } catch (const cipher_error& e) {
        cout << "Тест " << test_name << "... ПРОЙДЕН: исключение '" << e.what() << "'\n";
        return true;
    } catch (...) {
        cout << "Тест " << test_name << "... НЕ ПРОЙДЕН: неожиданное исключение\n";
        return false;
    }
}

bool expect_result(const string& test_name, const string& actual, const string& expected) {
    if (actual == expected) {
        cout << "Тест " << test_name << "... ПРОЙДЕН\n";
        return true;
    } else {
        cout << "Тест " << test_name << "... НЕ ПРОЙДЕН: ожидается '" << expected << "', получено '" << actual << "'\n";
        return false;
    }
}

// ========= ОСНОВНАЯ ФУНКЦИЯ ТЕСТИРОВАНИЯ =========

int main() {
    cout << "========================================\n";
    cout << "=== ТЕСТИРОВАНИЕ КОНСТРУКТОРА TableRouteCipher ===\n";
    cout << "========================================\n";

    try { TableRouteCipher(3); cout << "Тест 4.1: Верный ключ 3... ПРОЙДЕН\n"; } catch (...) { cout << "Тест 4.1: Верный ключ 3... НЕ ПРОЙДЕН\n"; }
    try { TableRouteCipher(10); cout << "Тест 4.2: Ключ больше длины текста 10... ПРОЙДЕН\n"; } catch (...) { cout << "Тест 4.2: Ключ больше длины текста 10... НЕ ПРОЙДЕН\n"; }
    expect_exception("4.3: В ключе отрицательное число -5", [](){ TableRouteCipher(-5); });
    expect_exception("4.4: В ключе ноль 0", [](){ TableRouteCipher(0); });
    try { TableRouteCipher(1); cout << "Тест 4.5: Ключ равен единице 1... ПРОЙДЕН\n"; } catch (const cipher_error& e) { cout << "Тест 4.5: Ключ равен единице 1... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }

    cout << "\n========================================\n";
    cout << "=== ТЕСТИРОВАНИЕ МЕТОДА encrypt ===\n";
    cout << "========================================\n";

    try { expect_result("5.1: Строка из прописных букв", TableRouteCipher(3).encrypt("PROGRAMMING"), "PGMNRRMGOAI"); } catch (const cipher_error& e) { cout << "Тест 5.1... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }
    try { expect_result("5.2: Строка из строчных букв", TableRouteCipher(3).encrypt("programming"), "pgmnrrmgoai"); } catch (const cipher_error& e) { cout << "Тест 5.2... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }
    try { expect_result("5.3: Строка с пробелами", TableRouteCipher(3).encrypt("PRO GRAMMING"), "PGMNRRMGOAI"); } catch (const cipher_error& e) { cout << "Тест 5.3... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }
    try { TableRouteCipher(3).encrypt("ALGORITHM2025"); cout << "Тест 5.4... НЕ ПРОЙДЕН\n"; } catch (const cipher_error&) { cout << "Тест 5.4: Строка с цифрами... ПРОЙДЕН: исключение\n"; }
    try { TableRouteCipher(3).encrypt(""); cout << "Тест 5.5... НЕ ПРОЙДЕН\n"; } catch (const cipher_error&) { cout << "Тест 5.5: Пустой текст... ПРОЙДЕН: исключение\n"; }
    try { TableRouteCipher(2).encrypt("123 !@#"); cout << "Тест 5.6... НЕ ПРОЙДЕН\n"; } catch (const cipher_error&) { cout << "Тест 5.6: Нет букв... ПРОЙДЕН: исключение\n"; }
    try { TableRouteCipher(6).encrypt("HELLO"); cout << "Тест 5.7... НЕ ПРОЙДЕН\n"; } catch (const cipher_error&) { cout << "Тест 5.7: Текст короче ключа... ПРОЙДЕН: исключение\n"; }
    try { expect_result("5.8: Текст равен ключу", TableRouteCipher(5).encrypt("HELLO"), "HELLO"); } catch (const cipher_error& e) { cout << "Тест 5.8... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }
    try { expect_result("5.9: Длина текста кратна ключу", TableRouteCipher(3).encrypt("ABCDEFGHI"), "ADGBEHCFI"); } catch (const cipher_error& e) { cout << "Тест 5.9... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }

    cout << "\n========================================\n";
    cout << "=== ТЕСТИРОВАНИЕ МЕТОДА decrypt ===\n";
    cout << "========================================\n";

    try { expect_result("6.1: Верный текст", TableRouteCipher(3).decrypt("PGMNRRMGOAI"), "PROGRAMMING"); } catch (const cipher_error& e) { cout << "Тест 6.1... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }
    try { expect_result("6.2: Есть строчные буквы", TableRouteCipher(3).decrypt("pgmnrrmgoai"), "programming"); } catch (const cipher_error& e) { cout << "Тест 6.2... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }
    try { expect_result("6.3: Есть пробелы", TableRouteCipher(3).decrypt("P G M N R R M G O A I"), "PROGRAMMING"); } catch (const cipher_error& e) { cout << "Тест 6.3... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }
    try { TableRouteCipher(4).decrypt("CODE2026XYZ"); cout << "Тест 6.4... НЕ ПРОЙДЕН\n"; } catch (const cipher_error&) { cout << "Тест 6.4: Есть цифры... ПРОЙДЕН: исключение\n"; }
    try { TableRouteCipher(3).decrypt("HELLO,WORLD!"); cout << "Тест 6.5... НЕ ПРОЙДЕН\n"; } catch (const cipher_error&) { cout << "Тест 6.5: Знаки препинания... ПРОЙДЕН: исключение\n"; }
    try { TableRouteCipher(3).decrypt(""); cout << "Тест 6.6... НЕ ПРОЙДЕН\n"; } catch (const cipher_error&) { cout << "Тест 6.6: Пустой текст... ПРОЙДЕН: исключение\n"; }
    try { expect_result("6.7: Текст с пробелами", TableRouteCipher(3).decrypt("ADGBEHCFI"), "ABCDEFGHI"); } catch (const cipher_error& e) { cout << "Тест 6.7... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }
    try { TableRouteCipher(6).decrypt("HELLO"); cout << "Тест 6.8... НЕ ПРОЙДЕН\n"; } catch (const cipher_error&) { cout << "Тест 6.8: Неполная таблица... ПРОЙДЕН: исключение\n"; }
    try { expect_result("6.9: Длина текста кратна ключу", TableRouteCipher(3).decrypt("ADGBEHCFI"), "ABCDEFGHI"); } catch (const cipher_error& e) { cout << "Тест 6.9... ИСКЛЮЧЕНИЕ: " << e.what() << "\n"; }

    cout << "\n========================================\n";
    cout << "=== ДОПОЛНИТЕЛЬНЫЙ ТЕСТ: КЛЮЧ = 1 ===\n";
    cout << "========================================\n";

    try {
        TableRouteCipher shifr(1);
        string enc = shifr.encrypt("HELLO");
        string dec = shifr.decrypt(enc);
        if (enc == "HELLO" && dec == "HELLO") {
            cout << "Тест 7.1: Ключ = 1, шифрование и дешифрование... ПРОЙДЕН\n";
        } else {
            cout << "Тест 7.1: Ключ = 1... НЕ ПРОЙДЕН: encrypt='" << enc << "', decrypt='" << dec << "'\n";
        }
    } catch (const cipher_error& e) {
        cout << "Тест 7.1: Ключ = 1... ИСКЛЮЧЕНИЕ: " << e.what() << "\n";
    }

    cout << "\n========================================\n";
    cout << "ТЕСТИРОВАНИЕ ЗАВЕРШЕНО\n";
    cout << "========================================\n";

    return 0;
}
